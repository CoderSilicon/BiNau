<section class="min-h-screen bg-[#0a0a0a] text-white selection:bg-white selection:text-black p-8 md:p-24">
  <div class="max-w-4xl mx-auto space-y-16">
    
    <!-- HEADER -->
    <header class="space-y-2">
      <h1 class="text-6xl font-black tracking-tighter jetbrains-mono-800">Documentation</h1>
    </header>

    <!-- WHAT IT IS -->
    <article class="space-y-6">
      <h2 class="text-[#333] text-[10px] tracking-[0.5em] uppercase border-b border-[#1a1a1a] pb-2 jetbrains-mono-400">The Concept</h2>
      <p class="text-xl md:text-2xl text-[#aaa] leading-relaxed jetbrains-mono-300">
        BiNau is a high-performance binary auditor built for transparency. 
        It deconstructs files to reveal their true intent, showing exactly what a program 
        does behind the scenes without any hidden layers or bloat.
      </p>
    </article>

    <!-- INPUTS -->
    <article class="space-y-6">
      <h2 class="text-[#333] text-[10px] tracking-[0.5em] uppercase border-b border-[#1a1a1a] pb-2 jetbrains-mono-400">Input Requirements</h2>
      <div class="grid grid-cols-1 md:grid-cols-2 gap-12">
        <div class="space-y-4">
          <h3 class="text-white text-sm uppercase font-bold tracking-tight jetbrains-mono-400">Accepted Formats</h3>
          <ul class="text-slate-100 text-sm space-y-2">
            <li class="bg-slate-500">+ .exe (Windows Executable)</li>
            <li class="bg-slate-500">+ Under Development</li>
            <li class="bg-slate-500">+ Under Development</li>
          </ul>
        </div>
        <div class="space-y-4">
          <h3 class="text-white text-sm uppercase font-bold tracking-tight jetbrains-mono-400">Method</h3>
          <p class="text-[#666] text-sm leading-relaxed jetbrains-mono-300">
            Drag the binary from your local system and drop it into the central void. 
            The system handles the rest locally.
          </p>
        </div>
      </div>
    </article>

    <!-- LOGIC -->
    <article class="space-y-8">
      <h2 class="text-[#333] text-[10px] tracking-[0.5em] uppercase border-b border-[#1a1a1a] pb-2 jetbrains-mono-400">The Pipeline</h2>
      <div class="space-y-12">
        <div class="flex gap-8">
          <span class="text-white font-bold jetbrains-mono-400">01</span>
          <div>
            <h4 class="text-white uppercase text-sm mb-2 jetbrains-mono-400">Parsing</h4>
            <p class="text-[#666] text-sm jetbrains-mono-300">The C++ engine maps the file’s internal structure and identifies the Import Address Table.</p>
          </div>
        </div>
        <div class="flex gap-8 border-t border-[#1a1a1a] pt-8">
          <span class="text-white font-bold jetbrains-mono-400">02</span>
          <div>
            <h4 class="text-white uppercase text-sm mb-2 jetbrains-mono-400">Mapping</h4>
            <p class="text-[#666] text-sm jetbrains-mono-300">Every system call is tracked—internet requests, file modifications, or registry access.</p>
          </div>
        </div>
        <div class="flex gap-8 border-t border-[#1a1a1a] pt-8">
          <span class="text-white font-bold jetbrains-mono-400">03</span>
          <div>
            <h4 class="text-white uppercase text-sm mb-2 jetbrains-mono-400">Verdict</h4>
            <p class="text-[#666] text-sm jetbrains-mono-300">BiNau presents a clear permission matrix, highlighting sensitive actions for total transparency.</p>
          </div>
        </div>
      </div>
    </article>

  </div>
</section>