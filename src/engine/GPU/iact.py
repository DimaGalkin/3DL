# include at compile time script
import os

rel_path = os.path.dirname(__file__)

file = open(f"{rel_path}/kernel.cl", "r")
lines = file.read()
file.close()

include1 = open(f"{rel_path}/mathcl.h", "r")
lines = include1.read() + "\n" + lines
include1.close()

include2 = open(f"{rel_path}/types.h", "r")
lines = include2.read() + "\n" + lines
include2.close()

header = """
#ifndef __IACT_H__
#define __IACT_H__
const char* kernel_include_source = R"(
"""

end = """
)";
#endif
"""

lines = header + lines + end

file = open(f"{rel_path}/iact.h", "w")
file.write(lines)
file.close()