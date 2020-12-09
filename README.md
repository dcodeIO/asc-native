asc native
==========

This is an experiment compiling the AssemblyScript compiler to a native executable
by first compiling the AssemblyScript compiler to WebAssembly using the AssemblyScript
compiler compiled to JavaScript, then converting the emitted Wasm binary to C using
wasm2c and compiling it to native along a native frontend feeding it teh codez.

**Caveats:** Since AssemblyScript links with Binaryen compiled to WebAssembly, and as
such assumes 32-bit pointers everywhere, we need to compile Binaryen to 32-bit, in
turn compiling the entire thing to a 32-bit executable.

**Status:** It compiles and runs but doesn't yet feed teh codez, producing the following
perfectly expected abort:

```
abort: Missing standard library component: ArrayBuffer in src/program.ts at line 1467:19
```

Building on Windows
-------------------

**Why?** Because unnecessary complexity and stacking layers is kinda the theme here.
May also work on *nixes with -T and -A amended.

Step 0: Make sure you have

* Clang build tools for Visual Studio. I used VS 2019 and checkboxed the feature.
* CMake, Clang/LLVM, Node.js
* All the submodules

Step 1: Build the Binaryen submodule (32-bit)

```sh
cd binaryen
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -T ClangCL -A Win32
cmake --build .
```

Step 2: Build the WABT submodule

```sh
cd wabt
git submodule update --init
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Step 3: Build the AssemblyScript compiler to Wasm

```sh
cd assemblyscript
npm install
npm run asbuild:untouched
```

Step 4: Transpile the AssemblyScript compiler compiled to Wasm to C

```sh
wabt/bin/wasm2c assemblyscript/out/assemblyscript.untouched.wasm -o src/assemblyscript.c
```

Step 5: Post-process the generated C sources. Links `binaryen.xy` imports to the Binaryen DLL.

```sh
node scripts/post.js
```

Step 6: Build the project itself (32-bit)

```sh
mkdir build
cd build
cmake .. -T ClangCL -A Win32
cmake --build .
```

Step 7: Go find your shiny asc.exe.
