# Toon Dash C/Raylib Migration

This is a strict 1:1 translation of the Toon Dash endless runner game from Rust/Macroquad to C/Raylib.
The internal logic, gameplay loop, mechanics, track generation, and collision values are exactly the same as the Rust implementation.

## Requirements

1. **Emscripten Toolkit (emsdk):** 
   You must have Emscripten installed and activated in your environment to build WebAssembly.
   If you do not have it, install it:
   ```bash
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh
   ```
2. **Make & Git:** Used by the build script to download and compile Raylib.

## Building the Game

Simply run make in the root directory:
```bash
make
```

The Makefile will automatically:
1. Clone Raylib (if not inside the `raylib/` directory already).
2. Compile `libraylib.a` for `PLATFORM_WEB` (WebAssembly) using `emcc`.
3. Compile all C source files in the `src/` directory.
4. Preload the `assets/` directory into a `.data` file (emscripten virtual file system).
5. Output the files `toon_dash.js`, `toon_dash.wasm`, and `toon_dash.data` inside the `web/` folder.

## Running the Game locally

Because the game fetches `.wasm` and `.data` files, it must be served via a local web server (opening index.html directly from the filesystem will throw CORS/fetch errors).

```bash
cd web
python3 -m http.server 8080
```

Then navigate to `http://localhost:8080` in your browser.

## Structural Differences from Rust

1. **Object Orientation:** Rust methods on structs (`player.update()`) have been mapped to C functions taking pointers (`UpdatePlayer(&player)`).
2. **Platform Abstraction Layer:** The JavaScript PAL in `web/tv-pal.js` is identical, but instead of hooking into Macroquad's plugin architecture, it hooks directly into exposed Emscripten functions via the `Module.ccall` and `Module.cwrap` APIs or standard window definitions bridging to the compiled exports.
3. **GLTF/Models:** Macroquad needed a custom parsing loop for `.glb` models to work gracefully with Emscripten atlases. Raylib's `LoadModel()` correctly ingests `.glb` out of the box. *Note: If a bespoke parsing implementation is desired later for deeper hardware pipeline control, an alternative using `cgltf.h` is documented in `src/assets.c`.*
