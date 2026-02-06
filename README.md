# WasmEdge CI Validator

A minimal, production-oriented CLI tool for validating WebAssembly modules in CI/CD pipelines using the WasmEdge C API.

## Overview

This tool recursively scans a directory for `.wasm` files and validates them using WasmEdge's validator component. It ensures binaries are structurally correct and safe to execute without actually running them. This avoids the side effects and risks associated with "hello world" execution tests in CI.

## Build Instructions

### Prerequisites
- CMake 3.10+
- C++17 compliant compiler (GCC/Clang)

### Building
```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
./wasmedge-ci-validator <directory>
```

### Examples

Pass validation:
```bash
./wasmedge-ci-validator ./dist
# Output: Scanning directory: /abs/path/to/dist
# Exit Code: 0
```

Fail validation (if directory is missing):
```bash
./wasmedge-ci-validator ./missing-dir
# Output: Error: Path does not exist: ./missing-dir
# Exit Code: 1
```
