<script lang="ts">
    let fileName = $state('');
    
    function handleFileUpload(event: Event) {
        const target = event.target as HTMLInputElement;
        const file = target.files?.[0];
        if (file) {
            fileName = file.name;
            console.log('File uploaded:', file);
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
            console.log('File dropped:', file);
        }
    }
    
    function clearFile() {
        fileName = '';
    }
    
    function analyzeFile() {
        console.log('Analyzing file:', fileName);
        // Add your analysis logic here
    }
</script>

<main class="text-white bg-[#0a0a0a] min-h-screen">
    <div class="max-w-6xl mx-auto px-6 py-16">
        <!-- Hero Section -->
        <div class="text-center mb-16">
            <h1 class="text-5xl font-bold jetbrains-mono-600 mb-4">Binary Auditor || BiNau</h1>
            <p class="text-lg text-zinc-400 jetbrains-mono-400">The secure binary analysis and research tool</p>
        </div>

        <!-- Upload Section -->
        <div class="max-w-2xl mx-auto mb-20">
        {#if !fileName}
            <label 
                for="file-upload" 
                class="flex flex-col items-center justify-center w-full h-48 border border-zinc-600 cursor-pointer hover:border-zinc-600 hover:bg-[#111111] transition-all group"
                ondragover={handleDragOver}
                ondrop={handleDrop}
            >
                <div class="flex flex-col items-center justify-center">
                    <svg class="w-12 h-12 mb-4 text-zinc-600 group-hover:text-zinc-500 transition-colors" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="1.5" d="M7 16a4 4 0 01-.88-7.903A5 5 0 1115.9 6L16 6a5 5 0 011 9.9M15 13l-3-3m0 0l-3 3m3-3v12" />
                    </svg>
                    <p class="text-sm text-zinc-400 jetbrains-mono-400 mb-1">
                        Click to upload
                    </p>
                    <p class="text-xs text-zinc-600 jetbrains-mono-400">Upload the executable file in this box</p>
                </div>
                <input id="file-upload" type="file" class="hidden" onchange={handleFileUpload} />
            </label>
            {/if}
            
            {#if fileName}
                <div class="mt-6 p-4 bg-[#111111] rounded-lg border border-zinc-800">
                    <div class="flex items-center justify-between">
                        <div class="flex items-center gap-3 flex-1">
                            <svg class="w-5 h-5 text-zinc-500" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12h6m-6 4h6m2 5H7a2 2 0 01-2-2V5a2 2 0 012-2h5.586a1 1 0 01.707.293l5.414 5.414a1 1 0 01.293.707V19a2 2 0 01-2 2z" />
                            </svg>
                            <p class="text-sm text-zinc-300 jetbrains-mono-400 truncate">{fileName}</p>
                        </div>
                        <button 
                            aria-label="Clear file"
                            onclick={clearFile}
                            class="ml-3 text-zinc-500 hover:text-red-400 transition-colors"
                        >
                            <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12" />
                            </svg>
                        </button>
                    </div>
                    <button 
                        aria-label="Analyze binary"
                        onclick={analyzeFile}
                        class="w-full mt-4 px-4 py-2 bg-white text-black font-semibold rounded hover:bg-gray-200 transition-colors jetbrains-mono-600"
                    >
                        Analyze Binary
                    </button>
                </div>
            {/if}
        </div>

        <!-- Features Grid -->
        <div class="grid md:grid-cols-3 gap-9 mb-16">
            <div class="p-6 border-x border-zinc-300 transition-colors">
                <div class="w-10 h-10  flex items-center justify-center mb-4">
                    <svg class="w-6 h-6 text-gray-100" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m5.618-4.016A11.955 11.955 0 0112 2.944a11.955 11.955 0 01-8.618 3.04A12.02 12.02 0 003 9c0 5.591 3.824 10.29 9 11.622 5.176-1.332 9-6.03 9-11.622 0-1.042-.133-2.052-.382-3.016z" />
                    </svg>
                </div>
                <h3 class="text-lg font-semibold mb-2 jetbrains-mono-600">Security Analysis</h3>
                <p class="text-sm text-gray-400 jetbrains-mono-400">Deep scan for vulnerabilities, malware signatures, and security threats</p>
            </div>

            <div class="p-6  border-x border-zinc-300 transition-colors">
                <div class="w-10 h-10  flex items-center justify-center mb-4">
                    <svg class="w-6 h-6 text-gray-100" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M10 20l4-16m4 4l4 4-4 4M6 16l-4-4 4-4" />
                    </svg>
                </div>
                <h3 class="text-lg font-semibold mb-2 jetbrains-mono-600">Binary Inspection</h3>
                <p class="text-sm text-gray-400 jetbrains-mono-400">Examine file headers, sections, symbols, and compiled code structure</p>
            </div>

            <div class="p-6  border-x border-zinc-300 transition-colors">
                <div class="w-10 h-10  flex items-center justify-center mb-4">
                    <svg class="w-6 h-6 text-gray-100" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 19v-6a2 2 0 00-2-2H5a2 2 0 00-2 2v6a2 2 0 002 2h2a2 2 0 002-2zm0 0V9a2 2 0 012-2h2a2 2 0 012 2v10m-6 0a2 2 0 002 2h2a2 2 0 002-2m0 0V5a2 2 0 012-2h2a2 2 0 012 2v14a2 2 0 01-2 2h-2a2 2 0 01-2-2z" />
                    </svg>
                </div>
                <h3 class="text-lg font-semibold mb-2 jetbrains-mono-600">Detailed Reports</h3>
                <p class="text-sm text-gray-400 jetbrains-mono-400">Generate comprehensive audit reports with actionable insights</p>
            </div>
        </div>

        <!-- Stats/Info Section -->
        <div class="grid md:grid-cols-4 gap-4">
            <div class="p-4 bg-[#111111] border-x border-zinc-400  text-center">
                <div class="text-2xl font-bold mb-1 jetbrains-mono-600">100+</div>
                <div class="text-xs text-gray-500 jetbrains-mono-400">File Formats</div>
            </div>
            <div class="p-4 bg-[#111111] border-x border-zinc-400 text-center">
                <div class="text-2xl font-bold mb-1 jetbrains-mono-600">99.9%</div>
                <div class="text-xs text-gray-500 jetbrains-mono-400">Accuracy</div>
            </div>
            <div class="p-4 bg-[#111111] border-x border-zinc-400 text-center">
                <div class="text-2xl font-bold mb-1 jetbrains-mono-600">&lt;3s</div>
                <div class="text-xs text-gray-500 jetbrains-mono-400">Analysis Time</div>
            </div>
            <div class="p-4 bg-[#111111] border-x border-zinc-400 text-center">
                <div class="text-2xl font-bold mb-1 jetbrains-mono-600">Private</div>
                <div class="text-xs text-gray-500 jetbrains-mono-400">Zero Storage</div>
            </div>
        </div>
    </div>
</main>


<style>
    @import url('https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;600&display=swap');
    
    .jetbrains-mono-400 {
        font-family: 'JetBrains Mono', monospace;
        font-weight: 400;
    }
    
    .jetbrains-mono-600 {
        font-family: 'JetBrains Mono', monospace;
        font-weight: 600;
    }
</style>