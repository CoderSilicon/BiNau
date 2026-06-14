<script lang="ts">
	import { onMount } from 'svelte';

	// 1. ISOLATED WASM INSTANCE: Absolutely NO $state here!
	// This protects Emscripten's internal HEAPU8 getters from Svelte's Proxy wrapper.
	let wasmInstance: any = null;

	// 2. UI REACTIVITY: Use this flag to update the frontend buttons instead
	let isEngineReady = $state(false);

	let fileName = $state('');
	let selectedFile: File | null = $state(null);
	let terminalLines = $state<string[]>([]);

	onMount(() => {
		const init = async () => {
			if (typeof window !== 'undefined' && (window as any).createBiNauModule) {
				try {
					terminalLines.push('[SYSTEM] Initializing WebAssembly Virtual Runtime...');

					const rawModule = await (window as any).createBiNauModule({
						print: (text: string) => {
							terminalLines.push(text);
						},
						printErr: (text: string) => {
							terminalLines.push(`[ENGINE ERROR] ${text}`);
						}
					});

					// Cache the raw reference safely
					wasmInstance = rawModule;

					// Tell Svelte to update the UI buttons
					isEngineReady = true;

					console.log('BiNau Engine: Online');
					terminalLines.push('[SYSTEM] BiNau Engine Core: Online and ready.');
				} catch (e) {
					console.error('WASM Load Failure:', e);
					terminalLines.push(`[SYSTEM] Core Initialization Failed: ${e}`);
				}
			} else {
				terminalLines.push(
					'[SYSTEM] Initialization Error: createBiNauModule script loader targets not found.'
				);
			}
		};
		init();
	});

	function handleFileUpload(event: Event) {
		const target = event.target as HTMLInputElement;
		const file = target.files?.[0];
		if (file) {
			fileName = file.name;
			selectedFile = file;
			terminalLines.push(`[SYSTEM] File targets queued: ${file.name}`);
		}
	}

	function handleDragOver(event: DragEvent) {
		event.preventDefault();
	}

	function handleDrop(event: DragEvent) {
		event.preventDefault();
		const file = event.dataTransfer?.files[0];
		if (file) {
			fileName = file.name;
			selectedFile = file;
			terminalLines.push(`[SYSTEM] File targets dropped: ${file.name}`);
		}
	}

	function clearFile() {
		fileName = '';
		selectedFile = null;
		terminalLines = ['[SYSTEM] Workspace cleared. Standing by.'];
	}

	async function analyzeFile() {
		if (!selectedFile || !wasmInstance) {
			terminalLines.push(
				'[SYSTEM] Processing error: Engine is not ready or no target file selected.'
			);
			return;
		}

		terminalLines = []; // Clear previous run
		const mbSize = (selectedFile.size / (1024 * 1024)).toFixed(2);
		terminalLines.push(`[SYSTEM] Ingesting binary payload stream (${mbSize} MB)...`);

		try {
			const arrayBuffer = await selectedFile.arrayBuffer();
			const uint8Array = new Uint8Array(arrayBuffer);
			const numBytes = uint8Array.length;

			const dataPtr = wasmInstance._malloc(numBytes);

			if (dataPtr === 0) {
				terminalLines.push(`[CRASH] Out of Memory: File is too large for the allocated WASM heap.`);
				return;
			}

			// Because wasmInstance is no longer a Svelte Proxy, HEAPU8 will now work perfectly!
			wasmInstance.HEAPU8.set(uint8Array, dataPtr);

			setTimeout(() => {
				try {
					wasmInstance.ccall('analyze_binary', 'void', ['number', 'number'], [dataPtr, numBytes]);
				} catch (e) {
					terminalLines.push(`[CRASH] Engine error encountered during execution loop processing.`);
					console.error(e);
				} finally {
					wasmInstance._free(dataPtr);
				}
			}, 50);
		} catch (err) {
			terminalLines.push(`[CRASH] Failed to read file data into memory: ${err}`);
		}
	}
</script>

<main class="min-h-screen bg-[#0a0a0a] text-white">
	<div class="mx-auto max-w-6xl px-6 py-16">
		<div class="mb-16 text-center">
			<h1 class="jetbrains-mono-600 mb-4 text-5xl font-bold">Binary Auditor || BiNau</h1>
			<p class="jetbrains-mono-400 text-lg text-zinc-400">
				The secure binary analysis and research tool
			</p>
		</div>

		<div class="mx-auto mb-20 max-w-2xl">
			{#if !fileName}
				<label
					for="file-upload"
					class="group flex h-48 w-full cursor-pointer flex-col items-center justify-center border border-zinc-600 transition-all hover:border-zinc-600 hover:bg-[#111111]"
					ondragover={handleDragOver}
					ondrop={handleDrop}
				>
					<div class="flex flex-col items-center justify-center">
						<svg
							class="mb-4 h-12 w-12 text-zinc-600 transition-colors group-hover:text-zinc-500"
							fill="none"
							stroke="currentColor"
							viewBox="0 0 24 24"
						>
							<path
								stroke-linecap="round"
								stroke-linejoin="round"
								stroke-width="1.5"
								d="M7 16a4 4 0 01-.88-7.903A5 5 0 1115.9 6L16 6a5 5 0 011 9.9M15 13l-3-3m0 0l-3 3m3-3v12"
							/>
						</svg>
						<p class="jetbrains-mono-400 mb-1 text-sm text-zinc-400">Click to upload</p>
						<p class="jetbrains-mono-400 text-xs text-zinc-600">
							Upload the executable file in this box
						</p>
					</div>
					<input id="file-upload" type="file" class="hidden" onchange={handleFileUpload} />
				</label>
			{/if}

			{#if fileName}
				<div class="mt-6 rounded-lg border border-zinc-800 bg-[#111111] p-4">
					<div class="flex items-center justify-between">
						<div class="flex flex-1 items-center gap-3">
							<svg
								class="h-5 w-5 text-zinc-500"
								fill="none"
								stroke="currentColor"
								viewBox="0 0 24 24"
							>
								<path
									stroke-linecap="round"
									stroke-linejoin="round"
									stroke-width="2"
									d="M9 12h6m-6 4h6m2 5H7a2 2 0 01-2-2V5a2 2 0 012-2h5.586a1 1 0 01.707.293l5.414 5.414a1 1 0 01.293.707V19a2 2 0 01-2 2z"
								/>
							</svg>
							<p class="jetbrains-mono-400 truncate text-sm text-zinc-300">{fileName}</p>
						</div>
						<button
							aria-label="Clear file"
							onclick={clearFile}
							class="ml-3 text-zinc-500 transition-colors hover:text-red-400"
						>
							<svg class="h-5 w-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
								<path
									stroke-linecap="round"
									stroke-linejoin="round"
									stroke-width="2"
									d="M6 18L18 6M6 6l12 12"
								/>
							</svg>
						</button>
					</div>
					<button
						aria-label="Analyze binary"
						onclick={analyzeFile}
						disabled={!isEngineReady}
						class="jetbrains-mono-600 mt-4 w-full rounded bg-white px-4 py-2 font-semibold text-black transition-colors hover:bg-gray-200 disabled:cursor-not-allowed disabled:opacity-50"
					>
						{isEngineReady ? 'Analyze Binary' : 'Loading Engine...'}
					</button>
				</div>
			{/if}
		</div>

		{#if terminalLines.length > 0}
			<div
				class="mx-auto mb-12 max-w-4xl rounded-md border border-zinc-800 bg-[#050505] p-6 font-mono text-xs leading-relaxed text-emerald-400 shadow-2xl"
			>
				<div
					class="mb-3 flex items-center justify-between border-b border-zinc-900 pb-2 text-zinc-500"
				>
					<span>TERMINAL MONITOR AREA</span>
					<div class="flex gap-1.5">
						<span class="h-2 w-2 rounded-full bg-zinc-700"></span>
						<span class="h-2 w-2 rounded-full bg-zinc-700"></span>
						<span class="h-2 w-2 rounded-full bg-zinc-700"></span>
					</div>
				</div>
				<div class="scrollbar-thin h-96 space-y-1 overflow-y-auto">
					{#each terminalLines as line, i (i)}
						<div class="whitespace-pre-wrap">{line}</div>
					{/each}
				</div>
			</div>
		{/if}

		<div class="mb-16 grid gap-9 md:grid-cols-3">
			<div class="border-x border-zinc-300 p-6 transition-colors">
				<div class="mb-4 flex h-10 w-10 items-center justify-center">
					<svg class="h-6 w-6 text-gray-100" fill="none" stroke="currentColor" viewBox="0 0 24 24">
						<path
							stroke-linecap="round"
							stroke-linejoin="round"
							stroke-width="2"
							d="M9 12l2 2 4-4m5.618-4.016A11.955 11.955 0 0112 2.944a11.955 11.955 0 01-8.618 3.04A12.02 12.02 0 003 9c0 5.591 3.824 10.29 9 11.622 5.176-1.332 9-6.03 9-11.622 0-1.042-.133-2.052-.382-3.016z"
						/>
					</svg>
				</div>
				<h3 class="jetbrains-mono-600 mb-2 text-lg font-semibold">Security Analysis</h3>
				<p class="jetbrains-mono-400 text-sm text-gray-400">
					Deep scan for vulnerabilities, malware signatures, and security threats
				</p>
			</div>

			<div class="border-x border-zinc-300 p-6 transition-colors">
				<div class="mb-4 flex h-10 w-10 items-center justify-center">
					<svg class="h-6 w-6 text-gray-100" fill="none" stroke="currentColor" viewBox="0 0 24 24">
						<path
							stroke-linecap="round"
							stroke-linejoin="round"
							stroke-width="2"
							d="M10 20l4-16m4 4l4 4-4 4M6 16l-4-4 4-4"
						/>
					</svg>
				</div>
				<h3 class="jetbrains-mono-600 mb-2 text-lg font-semibold">Binary Inspection</h3>
				<p class="jetbrains-mono-400 text-sm text-gray-400">
					Examine file headers, sections, symbols, and compiled code structure
				</p>
			</div>

			<div class="border-x border-zinc-300 p-6 transition-colors">
				<div class="mb-4 flex h-10 w-10 items-center justify-center">
					<svg class="h-6 w-6 text-gray-100" fill="none" stroke="currentColor" viewBox="0 0 24 24">
						<path
							stroke-linecap="round"
							stroke-linejoin="round"
							stroke-width="2"
							d="M9 19v-6a2 2 0 00-2-2H5a2 2 0 00-2 2v6a2 2 0 002 2h2a2 2 0 002-2zm0 0V9a2 2 0 012-2h2a2 2 0 012 2v10m-6 0a2 2 0 002 2h2a2 2 0 002-2m0 0V5a2 2 0 012-2h2a2 2 0 012 2v14a2 2 0 01-2 2h-2a2 2 0 01-2-2z"
						/>
					</svg>
				</div>
				<h3 class="jetbrains-mono-600 mb-2 text-lg font-semibold">Detailed Reports</h3>
				<p class="jetbrains-mono-400 text-sm text-gray-400">
					Generate comprehensive audit reports with actionable insights
				</p>
			</div>
		</div>

		<div class="grid gap-4 md:grid-cols-4">
			<div class="border-x border-zinc-400 bg-[#111111] p-4 text-center">
				<div class="jetbrains-mono-600 mb-1 text-2xl font-bold">100+</div>
				<div class="jetbrains-mono-400 text-xs text-gray-500">File Formats</div>
			</div>
			<div class="border-x border-zinc-400 bg-[#111111] p-4 text-center">
				<div class="jetbrains-mono-600 mb-1 text-2xl font-bold">99.9%</div>
				<div class="jetbrains-mono-400 text-xs text-gray-500">Accuracy</div>
			</div>
			<div class="border-x border-zinc-400 bg-[#111111] p-4 text-center">
				<div class="jetbrains-mono-600 mb-1 text-2xl font-bold">&lt;3s</div>
				<div class="jetbrains-mono-400 text-xs text-gray-500">Analysis Time</div>
			</div>
			<div class="border-x border-zinc-400 bg-[#111111] p-4 text-center">
				<div class="jetbrains-mono-600 mb-1 text-2xl font-bold">Private</div>
				<div class="jetbrains-mono-400 text-xs text-gray-500">Zero Storage</div>
			</div>
		</div>
	</div>
</main>
