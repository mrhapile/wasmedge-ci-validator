cmake_minimum_required(VERSION 3.10)
project(wasmedge-ci-validator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use target_include_directories instead of include_directories for better scoping
# include_directories(include)

# Common paths for WasmEdge
set(WASMEDGE_SEARCH_PATHS
    "$ENV{HOME}/.wasmedge"
    "/usr/local"
    "/usr"
)

# Find Include
find_path(WASMEDGE_INCLUDE_DIR NAMES wasmedge/wasmedge.h
    PATHS ${WASMEDGE_SEARCH_PATHS}
    PATH_SUFFIXES include
)

# Find Library
find_library(WASMEDGE_LIB NAMES wasmedge
    PATHS ${WASMEDGE_SEARCH_PATHS}
    PATH_SUFFIXES lib
)

if (WASMEDGE_INCLUDE_DIR AND WASMEDGE_LIB)
    message(STATUS "Found WasmEdge include: ${WASMEDGE_INCLUDE_DIR}")
    message(STATUS "Found WasmEdge lib: ${WASMEDGE_LIB}")
    
    if (NOT TARGET WasmEdge::wasmedge_c)
        add_library(WasmEdge::wasmedge_c UNKNOWN IMPORTED)
        set_target_properties(WasmEdge::wasmedge_c PROPERTIES
            IMPORTED_LOCATION "${WASMEDGE_LIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${WASMEDGE_INCLUDE_DIR}"
        )
    endif()
else()
    message(FATAL_ERROR "WasmEdge not found. Please install it to ~/.wasmedge or /usr/local")
endif()

add_executable(wasmedge-ci-validator
    src/main.cpp
    src/cli.cpp
    src/scanner.cpp
    src/validator.cpp
)

# Modern CMake: Include directories attached to the target
target_include_directories(wasmedge-ci-validator PRIVATE include)

# Link WasmEdge
target_link_libraries(wasmedge-ci-validator PRIVATE WasmEdge::wasmedge_c)
