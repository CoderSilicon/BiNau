<script>
	import logo from '$lib/assets/favicon.svg';

	// This state controls whether the mobile menu is visible
	let isMenuOpen = $state(false);

	// Function to flip the switch
	function toggleMenu() {
		isMenuOpen = !isMenuOpen;
	}
</script>

<!-- The 'relative' class ensures the dropdown attaches perfectly below the navbar -->
<nav class="relative z-50 w-full border-b border-[#222] bg-[#0a0a0a] font-mono text-white">
	<div class="mx-auto flex h-16 max-w-7xl items-stretch justify-between px-4">
		<!-- Branding: BINAU -->
		<div class="flex items-center border-r border-[#222] pr-6">
			<img src={logo} alt="Logo" class="mr-2 h-6 w-6" />
			<span class="text-2xl font-bold tracking-tighter">BiNau</span>
		</div>

		<!-- Desktop Menu (Hidden on small screens) -->
		<div class="hidden items-stretch md:flex">
			<a
				href="/"
				class="flex items-center border-r border-[#222] px-8 transition-colors hover:bg-[#1a1a1a]"
				>Home</a
			>
			<a
				href="/about"
				class="flex items-center border-r border-[#222] px-8 transition-colors hover:bg-[#1a1a1a]"
				>About</a
			>
			<a
				href="/docs"
				class="flex items-center border-r border-[#222] px-8 transition-colors hover:bg-[#1a1a1a]"
				>Docs</a
			>
			<a href="/contact" class="flex items-center px-8 transition-colors hover:bg-[#1a1a1a]"
				>Contact</a
			>
		</div>

		<!-- Mobile Toggle Button (Hidden on large screens) -->
		<div class="flex items-center md:hidden">
			<button
				onclick={toggleMenu}
				class="border border-[#333] px-4 py-2 text-xs font-bold tracking-widest uppercase transition-colors hover:bg-[#1a1a1a]"
			>
				<!-- Changes text dynamically based on the state -->
				{isMenuOpen ? 'CLOSE // X' : 'MENU // +'}
			</button>
		</div>
	</div>

	<!-- Mobile Dropdown Menu -->
	<!-- Only renders in the DOM if isMenuOpen is true -->
	{#if isMenuOpen}
		<div
			class="absolute top-16 left-0 flex w-full flex-col border-b border-[#222] bg-[#0a0a0a] shadow-2xl md:hidden"
		>
			<!-- Added onclick={toggleMenu} so the menu closes automatically when a user clicks a link -->
			<a
				href="/"
				onclick={toggleMenu}
				class="border-b border-[#1a1a1a] p-5 text-sm tracking-widest transition-colors hover:bg-[#111]"
				>01_HOME</a
			>
			<a
				href="/about"
				onclick={toggleMenu}
				class="border-b border-[#1a1a1a] p-5 text-sm tracking-widest transition-colors hover:bg-[#111]"
				>02_ABOUT</a
			>
			<a
				href="/docs"
				onclick={toggleMenu}
				class="border-b border-[#1a1a1a] p-5 text-sm tracking-widest transition-colors hover:bg-[#111]"
				>03_DOCS</a
			>
			<a
				href="/contact"
				onclick={toggleMenu}
				class="p-5 text-sm tracking-widest transition-colors hover:bg-[#111]">04_CONTACT</a
			>
		</div>
	{/if}
</nav>
