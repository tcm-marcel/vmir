#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>
#include <exception>

#include "vmir.h"

namespace vmir {

class IRUnit {
  public:
  IRUnit(size_t mem_size = MegaByte(64),
         size_t r_size = MegaByte(1),
         size_t a_size = MegaByte(1));

  ~IRUnit();

  void SetLogLevel(unsigned int level);

  void Load(const std::vector<char> bitcode);

  int Run(unsigned int argc = 0, char** argv = nullptr);

  private:
  std::unique_ptr<char> memory_;
  ir_unit_t *ir_unit_;

  static unsigned int MegaByte(unsigned int byte) {
    return byte * 1024 * 1024;
  }
};


class ExternalFunctionResolver {
 public:
  virtual vm_ext_function_t *ResolveFunction(const char *function, void *opaque) = 0;

  static vm_ext_function_t *GlobalResolveFunction(const char *function, void *opaque);

  static std::unique_ptr<ExternalFunctionResolver> global_resolver;
};


class FunctionWrapper : public ExternalFunctionResolver {
 public:
  vm_ext_function_t *ResolveFunction(const char *function, void *opaque) override;

  void AddFunction(std::string name, vm_ext_function_t* wrapper);

 private:
  std::unordered_map<std::string, vm_ext_function_t*> functions_;
};


template<typename function_t, function_t function>
int ExternalFunctionWrapper(void *ret, const void *regs, struct ir_unit *iu) {
  // get arguments here
  void* param_ptr = vmir_vm_ptr(&regs, iu);

  ret = function(&param_ptr);
}


/*
// from https://stackoverflow.com/questions/36492029/c-function-call-wrapper-with-function-as-template-argument
template<typename function_t, function_t function, typename... Args>
int ExternalFunctionWrapper(void *ret, const void *regs, struct ir_unit *iu) {
  // get arguments here

  void* param_ptr = vmir_vm_ptr(regs[i], iu);

  // result type: std::result_of<function_t(Args...)>::type
  ret = function(std::forward<Args>(args)...);
}
#define WRAPIT(FUNC) wrapper<decltype(&FUNC), &FUNC>
*/

} // namespace vmir