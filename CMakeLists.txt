cmake_minimum_required(VERSION 3.10)
project(wasmedge-ci-validator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Attempt to find WasmEdge via find_package first (for system installs with config)
# If that fails, fall back to manual verification for typical user installs (~/.wasmedge)

# Common paths
set(WASMEDGE_SEARCH_PATHS
    "$ENV{HOME}/.wasmedge"
    "/usr/local"
    "/usr"
)

find_path(WASMEDGE_INCLUDE_DIR NAMES wasmedge/wasmedge.h
    PATHS ${WASMEDGE_SEARCH_PATHS}
    PATH_SUFFIXES include
)

find_library(WASMEDGE_LIB NAMES wasmedge
    PATHS ${WASMEDGE_SEARCH_PATHS}
    PATH_SUFFIXES lib
)

if (WASMEDGE_INCLUDE_DIR AND WASMEDGE_LIB)
    message(STATUS "Found WasmEdge include: ${WASMEDGE_INCLUDE_DIR}")
    message(STATUS "Found WasmEdge lib: ${WASMEDGE_LIB}")
    
    # Define the target if not already defined (by a successful find_package)
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

target_link_libraries(wasmedge-ci-validator PRIVATE WasmEdge::wasmedge_c)
