#pragma once

namespace WasmEdgeCI {

/**
 * @brief Main entry point for the CLI logic.
 * 
 * @param argc Argument count
 * @param argv Argument values
 * @return int Exit code (0 for success, non-zero for failure)
 */
int runCLI(int argc, char* argv[]);

} // namespace WasmEdgeCI
