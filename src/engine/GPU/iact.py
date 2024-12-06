# library for detecting relative path
import os

# get path relative to path of compile script
rel_path = os.path.dirname(__file__)

# read gpu kernels and supporting functions
cl_source = open(f"{rel_path}/kernel.c", "r")
gpu_source_code = cl_source.read()
cl_source.close()

# load custom maths library
math_cllib = open(f"{rel_path}/mathcl.h", "r")
gpu_source_code = math_cllib.read() + "\n" + gpu_source_code
math_cllib.close()

# load types defined for CPU - GPU interfacing
typedefs = open(f"{rel_path}/types.h", "r")
gpu_source_code = typedefs.read() + "\n" + gpu_source_code
typedefs.close()

# include guard and var name to read from cpp code
header = """
#ifndef __IACT_H__
#define __IACT_H__
const char* kernel_include_source = R"(
#define __IACT_H__
"""

# end include guard and string containing GPU code
end = """
)";
#endif
"""

# concat three files into one include
gpu_source_code = header + gpu_source_code + end

# write single compile file out to be read by cpp
out_gpu_source = open(f"{rel_path}/iact.h", "w")
out_gpu_source.write(gpu_source_code)
out_gpu_source.close()
