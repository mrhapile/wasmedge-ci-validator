#include "scanner.h"
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace WasmEdgeCI {

std::vector<std::string> scanDirectory(const std::string &directoryPath) {
  std::vector<std::string> wasmFiles;

  // Safety check mostly handled by CLI, but good to have
  try {
    for (const auto &entry : fs::recursive_directory_iterator(directoryPath)) {
      if (entry.is_regular_file()) {
        if (entry.path().extension() == ".wasm") {
          try {
            wasmFiles.push_back(fs::absolute(entry.path()).string());
          } catch (const std::exception &e) {
            std::cerr << "Warning: Failed to resolve path for " << entry.path()
                      << ": " << e.what() << std::endl;
          }
        }
      }
    }
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Error scanning directory: " << e.what() << std::endl;
    throw; // Let caller handle strict failure if needed, or return empty
  }
  return wasmFiles;
}

} // namespace WasmEdgeCI
