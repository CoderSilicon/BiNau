# PROJECT: Binary Auditor
## Subtitle: The Ghost in the Machine | Abstract Binary Auditor

### 1. MISSION STATEMENT
To transform the opaque complexity of compiled software into a transparent, minimalist visual narrative. NEUROSCAN allows users—both technical and non-technical—to understand the "intent" of an executable without running it.

---

### 2. CORE FUNCTIONALITY
The application operates on a "Drop-and-Reveal" mechanic:
* **Ingestion:** User drags an `.exe` (PE) or Linux Binary (ELF) into the viewport.
* **Static Deconstruction:** The engine parses the file's Import Address Table (IAT) and section headers.
* **Semantic Mapping:** Machine-level API calls are mapped to human-level permissions.
* **Visual Genesis:** A 3D geometric "soul" is generated, representing the program's complexity and risk.

---

### 3. THE ANALYTIC ENGINE (BACKEND)
Driven by a WebAssembly (Wasm) core written in C++ for bare-metal speed.

| Analytic Pillar | Data Source | Logic |
| :--- | :--- | :--- |
| **Network Footprint** | `ws2_32.dll` / `socket` | Detects calls for data transmission or remote listening. |
| **System Access** | `kernel32.dll` | Identifies filesystem manipulation or process injection. |
| **Identity/Privacy** | `advapi32.dll` | Detects registry access or credential retrieval. |
| **Encryption** | `crypt32.dll` | Identifies if the app hides its data or encrypts user files. |

---

### 4. DESIGN LANGUAGE (ZEN UI)
The interface is a "Digital Void"—zero clutter, high impact.
* **The Hero:** A single, central 3D wireframe (Three.js).
    * *Stable Cube:* Low permissions, safe profile.
    * *Jagged Icosahedron:* High system access, complex logic.
    * *Pulsing Red Core:* Hidden/Obfuscated sections (Potential Malware).
* **Typography:** Monospace for technical data; High-kerning Sans-Serif for summaries.
* **Color Palette:** Deep Slate (`#0a0a0a`), Electric Cyan (`#00f2ff`), and Warning Amber (`#ffb300`).

---

### 5. TECHNICAL STACK
* **Frontend:** Vanilla JavaScript (ES6+), Three.js (WebGL).
* **Processing:** C++ compiled via Emscripten to WebAssembly.
* **Parsing Logic:** Manual PE/ELF header traversal (No heavy external libraries).
* **State Management:** Minimalist Pub/Sub pattern.

---

### 6. DEVELOPMENT ROADMAP
1.  **Phase Alpha (The Parser):** Build the Wasm module to extract strings and function names from raw bytes.
2.  **Phase Beta (The Visualizer):** Connect the parser output to Three.js vertex shaders to morph geometry.
3.  **Phase Gamma (The Interpreter):** Create the JSON "dictionary" that translates `CreateRemoteThread` into "Potentially Dangerous System Access."
4.  **Phase Delta (The Polish):** Implement glassmorphism UI overlays and smooth transitions.