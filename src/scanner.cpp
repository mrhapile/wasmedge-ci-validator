#include "scanner.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace WasmEdgeCI {

std::vector<std::string> scanDirectory(const std::string &directoryPath) {
  std::vector<std::string> wasmFiles;
  // We assume the path existence and directory check was handled by the CLI
  // entry point but recursive_directory_iterator throws if path doesn't exist,
  // so implicit check exists.

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
  return wasmFiles;
}

} // namespace WasmEdgeCI
