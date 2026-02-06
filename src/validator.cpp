#include "validator.h"
#include <wasmedge/wasmedge.h>

namespace WasmEdgeCI {

ValidationResult validateWasmFile(const std::string &filePath) {
  // 1. Configure
  WasmEdge_ConfigureContext *conf = WasmEdge_ConfigureCreate();

  // 2. VM Create
  WasmEdge_VMContext *vm = WasmEdge_VMCreate(conf, NULL);

  ValidationResult result = {true, ""};

  // 3. Load from file
  WasmEdge_Result res = WasmEdge_VMLoadWasmFromFile(vm, filePath.c_str());
  if (!WasmEdge_ResultOK(res)) {
    result.valid = false;
    result.message = WasmEdge_ResultGetMessage(res);

    // Cleanup on load failure
    WasmEdge_VMDelete(vm);
    WasmEdge_ConfigureDelete(conf);
    return result;
  }

  // 4. Validate
  res = WasmEdge_VMValidate(vm);
  if (!WasmEdge_ResultOK(res)) {
    result.valid = false;
    result.message = WasmEdge_ResultGetMessage(res);
  }

  // 5. Cleanup
  WasmEdge_VMDelete(vm);
  WasmEdge_ConfigureDelete(conf);

  return result;
}

} // namespace WasmEdgeCI
