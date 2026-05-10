<script lang="ts">
	import './layout.css';
	import './fonts.scss';
	import favicon from '$lib/assets/favicon.ico';
	import Header from '../components/header.svelte';
	import Footer from '../components/footer.svelte';

	let { children } = $props();
</script>

<svelte:head><link rel="icon" href={favicon} /></svelte:head>
<section class="min-h-screen bg-[#0a0a0a] text-white selection:bg-white selection:text-black antialiased">
	<Header />
	{@render children()}
	<Footer />
</section>
