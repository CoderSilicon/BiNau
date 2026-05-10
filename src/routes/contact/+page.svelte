<script>
	let email = $state('');
	let message = $state('');

	// Computes the mailto link dynamically
	let mailtoLink = $derived(
		`mailto:your-email@example.com?subject=InquiryofBinaryAuditor&body=${encodeURIComponent(message)}%0D%0A%0D%0AFrom: ${email}`
	);
</script>

<section
	class="min-h-screen bg-[#0a0a0a] p-6 font-mono text-white selection:bg-white selection:text-black md:p-20 lg:p-32"
>
	<div class="mx-auto max-w-5xl">
		<!-- HEADER -->
		<header class="mb-24 border-b border-[#1a1a1a] pb-6">
			<h1 class="jetbrains-mono-800 text-7xl leading-none font-black tracking-tighter md:text-9xl">
				Contact
			</h1>
		</header>

		<div class="grid grid-cols-1 gap-24 lg:grid-cols-12">
			<!-- RIGHT: THE INTERFACE -->
			<div class="lg:col-span-7">
				<div class="space-y-1">
					<!-- EMAIL INPUT -->
					<div class="border border-[#1a1a1a] p-6 transition-colors focus-within:bg-[#0e0e0e]">
						<label for="email" class="mb-2 block text-[10px] text-[#444] uppercase">Your Email</label>
						<input
							bind:value={email}
							type="email"
							placeholder="user@domain.com"
							class="w-full border-none bg-transparent text-sm text-white outline-none placeholder:text-[#222]"
						/>
					</div>

					<!-- MESSAGE INPUT -->
					<div class="border border-[#1a1a1a] p-6 transition-colors focus-within:bg-[#0e0e0e]">
						<label for="message" class="mb-2 block text-[10px] text-[#444] uppercase">Your Message</label>
						<textarea
							bind:value={message}
							rows="6"
							placeholder="Enter the message you want to say..."
							class="w-full resize-none border-none bg-transparent text-sm text-white outline-none placeholder:text-[#222]"
						></textarea>
					</div>

					<!-- EXECUTE BUTTON -->
					<a
					/* eslint-disable-next-line svelte/no-navigation-without-resolve */
						href={mailtoLink}
						class="group block w-full border border-[#1a1a1a] p-6 text-center transition-all hover:bg-white hover:text-black"
					>
						<span class="text-xs font-bold tracking-[0.3em] uppercase">Send Message</span>
					</a>
				</div>
			</div>
		</div>

	</div>
</section>
