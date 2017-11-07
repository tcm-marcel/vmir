#include <vector>
#include <fstream>

#include "vmir_lib.h"

std::vector<char> ReadFileToMemory(std::string file_path) {
  std::ifstream file(file_path, std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  file.read(buffer.data(), size);

  if (file.bad())
  {
    throw std::runtime_error("couldn't read file");
  }

  return std::move(buffer);
}

extern "C" {
int add(int a, int b) {
  printf("%d + %d = %d\n", a, b, a + b);
  return a + b;
}
}

using add_t =  int(int a, int b);

int AddWrapper(void *ret, const void *regs, struct ir_unit *iu) {
  // get arguments here
  // void* param_ptr = vmir_vm_ptr(regs, iu);
  // vmir_vm_arg32
  // vmir_vm_ret32(ret,
  // vmir_vm_arg64
  // vmir_vm_ptr_nullchk
  // vmir_vm_arg_dbl douple argument
  // vmir_vm_arg_func
  // vmir_vm_retptr
  // vmir_vm_ret64
  // vmir_vm_arg_flt float

  ret = 0;
  add(1, 2);
  return 0;
}

static int
printf_wrapper(void *ret, const void *rf, ir_unit_t *iu)
{
  const char *fmt = (const char*)vmir_vm_ptr(&rf, iu);

  std::printf(fmt);

  vmir_vm_ret32(ret, 0);
  return 0;
}



int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return 1;
  }

  // configure external functions
  //vm_ext_function_t *add_wrapper = (vm_ext_function_t*)(&vmir::ExternalFunctionWrapper<add_t, add>);
  vm_ext_function_t  *add_wrapper = &AddWrapper;

  auto resolver = std::make_unique<vmir::FunctionWrapper>();
  //resolver->AddFunction("add", add_wrapper);
  resolver->AddFunction("printf", printf_wrapper);
  vmir::ExternalFunctionResolver::global_resolver = std::move(resolver);

  vmir::IRUnit ir_unit;
  ir_unit.SetLogLevel(VMIR_LOG_DEBUG);
  std::vector<char> bitcode = ReadFileToMemory(argv[1]);

  ir_unit.Load(bitcode);

  int ret = ir_unit.Run();

  return ret;
}