# WasmEdge CI Validator

A production-oriented CLI tool for validating WebAssembly modules in CI/CD pipelines using the WasmEdge C API.

## Overview

`wasmedge-ci-validator` recursively scans a directory for `.wasm` files and validates them using the [WasmEdge](https://wasmedge.org) runtime. It ensures binaries are structurally correct according to the Wasm standard without executing unsafe code or requiring a complex runtime environment.

## Features
- **Recursive Scanning**: Automatically discovers all `.wasm` files in a build directory.
- **Pure Validation**: Uses WasmEdge C API to check format and bytecode safety only.
- **CI Friendly**: Strict exit codes (`0`/`1`) and parseable summaries.
- **No Side Effects**: Does not instantiate modules or run `_start`.

## Build Instructions

### Prerequisites
- CMake 3.10+
- C++17 Compiler (GCC/Clang)
- WasmEdge installed (system-wide or in `~/.wasmedge`)

### Building
```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Run the validator on any directory:

```bash
./wasmedge-ci-validator ./dist
```

### Output Example (Success)
```text
Scanning directory: /path/to/dist
Found 3 WASM files.
✔ /path/to/dist/lib.wasm
✔ /path/to/dist/app.wasm
✔ /path/to/dist/utils.wasm

Summary:

Total WASM files: 3

Valid: 3

Invalid: 0
```
(Exit Code: 0)

### Output Example (Failure)
```text
Scanning directory: /path/to/dist
Found 2 WASM files.
✔ /path/to/dist/ok.wasm
✖ /path/to/dist/corrupt.wasm (magic header not detected)

Summary:

Total WASM files: 2

Valid: 1

Invalid: 1

Validation failed: 1 invalid WASM file found
```
(Exit Code: 1)

## CI Integration

### GitHub Actions
```yaml
steps:
  - name: Install WasmEdge
    run: curl -sSf https://raw.githubusercontent.com/WasmEdge/WasmEdge/master/utils/install.sh | bash

  - name: Build Validator
    run: |
      cmake -S . -B build
      cmake --build build

  - name: Validate Artifacts
    run: ./build/wasmedge-ci-validator ./wasm_output_dir
```

## License
Apache-2.0
