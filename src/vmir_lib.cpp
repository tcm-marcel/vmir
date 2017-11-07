#include <algorithm>


#include "vmir_lib.h"


namespace vmir {


IRUnit::IRUnit(size_t mem_size,
               size_t r_size,
               size_t a_size)
    : memory_(std::unique_ptr<char>((char*) calloc(1, mem_size))) {
  // create ir unit
  ir_unit_ = vmir_create(memory_.get(), MegaByte(64), MegaByte(1), MegaByte(1), nullptr);
}


IRUnit::~IRUnit() {
  vmir_destroy(ir_unit_);
}


void IRUnit::Load(const std::vector<char> bitcode) {
  // set external function resolver if available
  if (ExternalFunctionResolver::global_resolver) {
    vmir_set_external_function_resolver(ir_unit_, &ExternalFunctionResolver::GlobalResolveFunction);
  }

  if (vmir_load(ir_unit_, (const uint8_t*) bitcode.data(), bitcode.size())) {
    throw std::runtime_error("couldn't ir_unit from bitcode");
  }
}


void IRUnit::SetLogLevel(unsigned int level) {
  vmir_set_log_level(ir_unit_, VMIR_LOG_DEBUG);
}


int IRUnit::Run(unsigned int argc, char** argv) {
  int return_value;
  vmir_run(ir_unit_, &return_value, argc, argv);

  return return_value;
}


vm_ext_function_t *ExternalFunctionResolver::GlobalResolveFunction(const char *function, void *opaque) {
  return global_resolver->ResolveFunction(function, opaque);
}

std::unique_ptr<ExternalFunctionResolver> ExternalFunctionResolver::global_resolver = nullptr;


vm_ext_function_t *FunctionWrapper::ResolveFunction(const char *name, void *opaque) {
  auto iterator = functions_.find(std::string(name));

  if (iterator != functions_.end()) {
    return iterator->second;
  }

  // otherwise just return NULL, the function can still be internal defined
  return NULL;
}


void FunctionWrapper::AddFunction(std::string name, vm_ext_function_t* wrapper) {
  functions_[name] = wrapper;
};


} // namespace vmir