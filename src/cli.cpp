#include "cli.h"
#include <filesystem>
#include <iostream>
#include <string>

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

    // Phase 1 Success Requirement
    std::cout << "Scanning directory: " << fs::absolute(dirPath).string()
              << std::endl;

  } catch (const fs::filesystem_error &e) {
    std::cerr << "Error accessing filesystem: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

} // namespace WasmEdgeCI
