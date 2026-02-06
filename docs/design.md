# WasmEdge CI Validator: Phase 0 Design Specification

## 1. Problem Statement

CI/CD pipelines frequently generate WebAssembly artifacts that need verification before deployment. Currently, ensuring a WASM binary is valid using the standard `wasmedge` CLI typically requires attempting to *execute* it or running a "hello world" function.

This is insufficient for CI contexts because:
1.  **Execution Risks:** Running untrusted or work-in-progress binaries can lead to infinite loops, crashes, or unintended side effects.
2.  **Missing Context:** CI environments may lack the specific input data or WASI environment variables required for execution, causing false negatives.
3.  **Inefficiency:** Validating a build output directory (e.g., `dist/` or `bin/`) requires writing complex shell scripts to loop over files.

This tool solves these issues by providing a dedicated, directory-based validator that strictly uses the WasmEdge C API to check generic validity without instantiation or execution.

## 2. Tool Definition

`wasmedge-ci-validator` is a standalone command-line utility that accepts a filesystem directory path as input. It recursively scans for `.wasm` files, validates them against the WasmEdge runtime specification, and prints a status report (PASS/FAIL) for each file. The tool exits with code `0` only if all detected WASM files are valid; otherwise, it exits with a non-zero status code to signal build failure in CI systems.

## 3. Scope

### In Scope
*   **Directory Scanning:** Recursive traversal of the target directory to locate all files with the `.wasm` extension.
*   **Pure Validation:** Using the WasmEdge C API (`WasmEdge_LoaderContext` and `WasmEdge_ValidatorContext`) to verify binary format and bytecode safety.
*   **CI Integration:** deterministic exit codes (0 for success, 1 for failure/error).
*   **Reporting:** Simple, parseable generic output to `stdout`/`stderr` indicating which specific files failed.

### Out of Scope
*   **Execution:** No WASM functions will be invoked, and no modules will be instantiated.
*   **WASI Support:** Interactions with the system interface (files, network, clocks) are ignored; only the WASM binary structure is checked.
*   **Configuration:** No support for config files (toml/json) or complex flags.
*   **Network:** No fetching modules from registries.
*   **Benchmarking:** No performance metrics or timing data collecting.

## 4. Success Criteria

The Phase 0 implementation is considered complete and successful when:
1.  The tool compiles to a single binary on standard POSIX systems (Linux/macOS).
2.  Running `wasmedge-ci-validator ./valid-wasm-dir` returns exit code **0**.
3.  Running `wasmedge-ci-validator ./mixed-wasm-dir` (containing corrupt/invalid WASM) detects the invalid files, prints error details, and returns exit code **1**.
4.  Running `wasmedge-ci-validator` on a path with no WASM files handles the case gracefully (e.g., warns and exits 0, or errors depending on final spec preference, defaulted to success if scanning empty).
5.  No "Hello World" or execution logic is triggered during the process.

## 5. High-Level Design

The application logic follows a linear pipeline:

1.  **CLI Driver:** Parses command-line arguments to validate that a single, readable usage path is provided.
2.  **Scanner Component:** Walks the provided directory path to build a list of target `.wasm` file paths.
3.  **Validator Component:** 
    *   Initializes WasmEdge configuration (disabling AOT/Interpreter optimizations not needed for validation).
    *   Iterates through the file list.
    *   Uses `WasmEdge_Loader` to check file format.
    *   Uses `WasmEdge_Validator` to check module correctness.
    *   Captures error messages for invalid modules.
4.  **Result Aggregator:** Collects pass/fail results and generates the final console output and process exit code.

## 6. Relation to WasmEdge CLI

This project serves as a focused reference implementation for "pure validation" using the WasmEdge SDK. It motivates the potential addition of a `validate` subcommand to the upstream `wasmedge` CLI by demonstrating the utility of checking binary correctness without execution. By keeping the codebase minimal and open, it provides a clear template for other developers integrating WasmEdge into custom CI workfloes.
