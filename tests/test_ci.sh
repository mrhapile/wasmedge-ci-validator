#!/bin/bash
set -e

# Configuration
TEST_DIR="test_artifacts"
TOOL="./build/wasmedge-ci-validator"
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

log() {
    echo -e "${GREEN}[TEST]${NC} $1"
}

fail() {
    echo -e "${RED}[FAIL]${NC} $1"
    exit 1
}

setup() {
    log "Setting up test environment..."
    rm -rf "$TEST_DIR"
    mkdir -p "$TEST_DIR"

    # 1. Create a VALID wasm file (minimal magic header)
    # \0asm\1\0\0\0
    printf '\x00\x61\x73\x6d\x01\x00\x00\x00' > "$TEST_DIR/valid.wasm"

    # 2. Create an INVALID wasm file (bad magic)
    echo "this is not wasm" > "$TEST_DIR/invalid.wasm"
}

cleanup() {
    log "Cleaning up..."
    rm -rf "$TEST_DIR"
}

test_all_valid() {
    log "Case 1: All valid inputs"
    mkdir -p "$TEST_DIR/case1"
    cp "$TEST_DIR/valid.wasm" "$TEST_DIR/case1/a.wasm"
    cp "$TEST_DIR/valid.wasm" "$TEST_DIR/case1/b.wasm"

    if ! $TOOL "$TEST_DIR/case1" > /dev/null; then
        fail "Tool failed on valid inputs, expected exit code 0"
    fi
    log "Case 1 Passed"
}

test_mixed_inputs() {
    log "Case 2: Mixed valid/invalid inputs"
    mkdir -p "$TEST_DIR/case2"
    cp "$TEST_DIR/valid.wasm" "$TEST_DIR/case2/good.wasm"
    cp "$TEST_DIR/invalid.wasm" "$TEST_DIR/case2/bad.wasm"

    # Capture output to check for summary
    OUTPUT=$($TOOL "$TEST_DIR/case2" 2>&1 || true)
    
    # We expect failure (exit code 1)
    if $TOOL "$TEST_DIR/case2" > /dev/null 2>&1; then
        fail "Tool succeeded on mixed inputs, expected exit code 1"
    fi

    # Check for correct output strings
    if [[ ! "$OUTPUT" == *"Total WASM files: 2"* ]]; then
        fail "Output missing correct total count"
    fi
    if [[ ! "$OUTPUT" == *"Invalid: 1"* ]]; then
        fail "Output missing correct invalid count"
    fi

    log "Case 2 Passed"
}

test_missing_dir() {
    log "Case 3: Missing directory"
    if $TOOL "./nonexistent_path_xyz" > /dev/null 2>&1; then
        fail "Tool succeeded on missing dir, expected exit code 1"
    fi
    log "Case 3 Passed"
}

# Run
setup
test_all_valid
test_mixed_inputs
test_missing_dir
cleanup

log "ALL TESTS PASSED"
