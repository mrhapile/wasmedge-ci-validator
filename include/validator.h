#pragma once
#include <string>

namespace WasmEdgeCI {

struct ValidationResult {
  bool valid;
  std::string message;
};

/**
 * @brief Validates a single WASM file using the WasmEdge C API.
 *
 * @param filePath Path to the .wasm file.
 * @return ValidationResult containing success status and error message (if
 * any).
 */
ValidationResult validateWasmFile(const std::string &filePath);

} // namespace WasmEdgeCI
