#pragma once
#include <string>
#include <vector>

namespace WasmEdgeCI {

/**
 * @brief Recursively scans the directory for .wasm files.
 *
 * @param directoryPath The path to scan.
 * @return std::vector<std::string> List of absolute paths to .wasm files.
 * @throws std::filesystem::filesystem_error if scanning fails.
 */
std::vector<std::string> scanDirectory(const std::string &directoryPath);

} // namespace WasmEdgeCI
