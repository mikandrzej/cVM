

FILENAME = "program.vms"
OUTPUT_FILENAME = "compiled.vmo"

instrutctionsDict = {
    "add": {
        "code": 0x40,
        "arguments": 3,
        "argumentTypes": [
            "uint8_t",
            "uint8_t",
            "int32_t"
        ]},
    "sub": {
        "code": 0x41,
        "arguments": 3,
        "argumentTypes": [
            "uint8_t",
            "uint8_t",
            "int32_t"
        ]},
    "div": {
        "code": 0x42,
        "arguments": 3,
        "argumentTypes": [
            "uint8_t",
            "uint8_t",
            "int32_t"
        ]},
    "mod": {
        "code": 0x43,
        "arguments": 3,
        "argumentTypes": [
            "uint8_t",
            "uint8_t",
            "int32_t"
        ]},
    "mul": {
        "code": 0x44,
        "arguments": 3,
        "argumentTypes": [
            "uint8_t",
            "uint8_t",
            "int32_t"
        ]},
    "pow": {
        "code": 0x45,
        "arguments": 3,
        "argumentTypes": [
            "uint8_t",
            "uint8_t",
            "int32_t"
        ]},
}

output = bytearray()

def convertArguments(args, argTypes, output):
    for k, arg in enumerate(args):
        argType = argTypes[k]

        if argType == "uint8_t":
                tempArg = int(arg)
                output.extend(bytes([tempArg]))
        elif argType == "uint16_t":
                tempArg = int(arg)
                output.extend(bytes([tempArg % 256, (tempArg >> 8) % 256]))
        elif argType == "uint32_t":
                tempArg = int(arg)
                output.extend(bytes([tempArg % 256, (tempArg >> 8) % 256, (tempArg >> 16) % 256, (tempArg >> 24) % 256]))
        elif argType == "int32_t":
                tempArg = int(arg)
                output.extend(bytes([tempArg % 256, (tempArg >> 8) % 256, (tempArg >> 16) % 256, (tempArg >> 24) % 256]))


with open(FILENAME, 'r') as f:
    lineNumber = 1

    while(True):
        line = f.readline()
        if line is None or line.strip() == '':
            break;

        lineData = line.split(" ")

        instrName = lineData[0].strip()
        instruction = instrutctionsDict.get(instrName)
        if instruction is None:
            print("Invalid instruction: \"" + instrName + "\" at line: " + str(lineNumber))
        args = lineData[1:]
        args = [x.strip().replace(',', "") for x in args]
        if instruction.get("arguments") != len(args):
            print("Invalid argument length, line: " + str(lineNumber))

        print("Line " + str(lineNumber) + " OK! Got instruction: " + instrName + " and arguments: " + str(args))

        output.extend(bytes([instruction.get("code")]))
        tempOutput = bytearray()
        convertArguments(args, instruction.get("argumentTypes"), tempOutput)

        lineNumber += 1

print (output)