![BiNauImage](/src/lib/assets/favicon.svg)
# BiNau or Binary Auditor

> **Static Binary Auditor & Security Report Engine**

## Overview

**Binary Auditor** aka BiNau with codename *"The Ghost in the Machine"*, is a high-performance binary auditor designed for deep static analysis report generator for executables. It deconstructs artifacts to map their **Bare Matrix**—identifying underlying "work" before execution. By leveraging a C++20 engine compiled to WebAssembly, BiNau provides near-native deconstruction speeds directly in the browser's volatile memory.

## Core Principles

* **Zero Execution:** Analysis is strictly static; the executable never enters an execution state.
* **Data Sovereignty:** 100% client-side. Artifacts are mirrored into a WASM heap and purged immediately post-audit.
* **Precision:** Used extreme lightweight software for best accuracy.

---

## Technical Stack

* **Engine:** C++20 / LLVM
* **Runtime:** Emscripten / WebAssembly (Modular ES6)
* **Frontend:** Svelte 5 (Runes) / Tailwind CSS
* **Architecture:** Cross-platform Automation (Node.js / Bash)

---



## NOTE

```text
[ STATUS: STILL IN DEVELOPMENT ]
[ VERSION: 1.0.0_ALPHA ]
[ ENGINE: BINAUX ]
[ ARCH: X86_64_WASM ]
```
---

**Developed by O<sup>3</sup>Foundry.**
