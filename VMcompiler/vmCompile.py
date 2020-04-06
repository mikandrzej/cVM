import sys

from vmSource import VMSource

FILENAME = "program.vms"
OUTPUT_FILENAME = "compiled.bin"

# vmSource = VMSource(["program.vms", "program2.vms"], OUTPUT_FILENAME)
# print(vmSource.bOutput)

if __name__ == '__main__':
    source = VMSource(sys.argv[1:], OUTPUT_FILENAME)
    print(source.bOutput)
