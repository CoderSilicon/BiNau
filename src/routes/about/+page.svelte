<script>
  import logo from "$lib/assets/favicon.svg"
</script>

<section class="min-h-screen bg-[#0a0a0a] text-white font-mono selection:bg-white selection:text-black">
  <div class="grid grid-cols-1 lg:grid-cols-12 min-h-screen">
    
    <!-- LEFT SIDE: STATIC BRANDING -->
    <aside class="lg:col-span-4 border-b lg:border-b-0 lg:border-r border-[#1a1a1a] p-12 md:p-20">
      <h1 class="text-5xl font-black tracking-tighter">
        BiNau
      </h1>
      <p class="mt-4 text-[#444] text-[10px] tracking-[0.4em]">
        Version αlpha
      </p>
      <img src={logo} alt="BiNau Logo" class="mt-12 w-64 h-64 object-contain ">
    </aside>

    <!-- RIGHT SIDE: CORE CONTENT -->
    <main class="lg:col-span-8 p-12 md:p-20 md:py-40 space-y-32">
      
      <!-- THE WHY -->
      <section class="max-w-2xl">
        <h2 class="text-[#333] text-[10px] tracking-[0.5em] uppercase mb-10">Concept</h2>
        <p class="text-2xl md:text-4xl text-[#eee] leading-tight font-light">
          BiNau is a high-performance tool built to show you exactly how a computer program works.
        </p>
        <p class="mt-8 text-[#777] text-lg leading-relaxed">
          Most software acts as a closed box which does not let you to know what is happening. BiNau allows you to open it by analyzing the program's internal structure, 
          giving you a clear view of its intent without any hidden layers or unnecessary complexity.
        </p>
      </section>

      <!-- THE WHO -->
      <section class="max-w-2xl pt-24 border-t border-[#1a1a1a]">
        <h2 class="text-[#333] text-[10px] tracking-[0.5em] uppercase mb-10">Creator</h2>
        <p class="text-2xl md:text-4xl text-[#eee] leading-tight font-light">
          Created by <span class="bg-gray-800 text-slate-400">@CoderSilicon</span>.
        </p>
        <p class="mt-8 text-[#777] text-lg leading-relaxed">
          @CoderSilicon is a developer and open-source enthusiast. With a deep interest in programming languages and software architecture, he/she created BiNau to empower developers and enthusiasts to understand the inner workings of software in a transparent and accessible way.
        </p>
      </section>
    </main>
  </div>
</section>