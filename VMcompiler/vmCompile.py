from VMcompiler.vmProgram import VMProgram
from VMcompiler.vmSource import VMSource

FILENAME = "program.vms"
OUTPUT_FILENAME = "compiled.bin"

vmSource = VMSource(FILENAME, OUTPUT_FILENAME)
print(vmSource.bOutput)
