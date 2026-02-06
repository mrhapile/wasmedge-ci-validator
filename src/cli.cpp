#include "cli.h"
#include "scanner.h"
#include "validator.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace WasmEdgeCI {

void printUsage(const char *programName) {
  std::cerr << "Usage: " << programName << " <directory>" << std::endl;
  std::cerr << "Validates WASM files in the specified directory." << std::endl;
}

int runCLI(int argc, char *argv[]) {
  // We expect exactly 2 arguments: program name + directory path
  if (argc != 2) {
    std::cerr << "Error: Invalid number of arguments." << std::endl;
    printUsage(argv[0]);
    return 1;
  }

  std::string pathStr = argv[1];
  fs::path dirPath(pathStr);

  try {
    if (!fs::exists(dirPath)) {
      std::cerr << "Error: Path does not exist: " << pathStr << std::endl;
      return 1;
    }

    if (!fs::is_directory(dirPath)) {
      std::cerr << "Error: Path is not a directory: " << pathStr << std::endl;
      printUsage(argv[0]);
      return 1;
    }

    // Phase 2: Scan
    std::vector<std::string> files =
        scanDirectory(fs::absolute(dirPath).string());

    if (files.empty()) {
      std::cout << "No .wasm files found in " << pathStr << std::endl;
      return 0;
    }

    std::cout << "Found " << files.size() << " WASM files." << std::endl;

    // Phase 3: Validate
    int failureCount = 0;
    int successCount = 0;

    for (const auto &file : files) {
      ValidationResult res = validateWasmFile(file);
      if (res.valid) {
        std::cout << "✔ " << file << std::endl;
        successCount++;
      } else {
        std::cout << "✖ " << file << " (" << res.message << ")" << std::endl;
        failureCount++;
      }
    }

    std::cout << "\nSummary:" << std::endl;
    std::cout << "\nTotal WASM files: " << files.size() << std::endl;
    std::cout << "\nValid: " << successCount << std::endl;
    std::cout << "\nInvalid: " << failureCount << std::endl;

    if (failureCount > 0) {
      std::cerr << "\nValidation failed: " << failureCount
                << " invalid WASM file" << (failureCount == 1 ? "" : "s")
                << " found" << std::endl;
      return 1;
    }

  } catch (const fs::filesystem_error &e) {
    std::cerr << "Error accessing filesystem: " << e.what() << std::endl;
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Unexpected error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

} // namespace WasmEdgeCI
