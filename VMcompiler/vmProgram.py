import re

class VMProgram(object):
    from VMcompiler.vmInstructions import instructionsDict as _instructionsDict

    def __init__(self, text: str):
        self.bProgram = bytearray()
        self.programID = 0xFFFF
        self.programLen = 0
        self.functions = {}
        self.calls = {}
        self.importFromText(text)
        self._replaceCallsOffsets()
        pass

    def importFromText(self, text: str):
        lines = text.splitlines(keepends=False)
        for line in lines:
            parseRes = self._parseLine(line)
            if parseRes == None:
                continue
            elif parseRes == False:
                raise Exception("Compilation failed.")

            if type(parseRes) is bytearray:
                self.bProgram.extend(parseRes)

        self.programLen = len(self.bProgram)


    def _parseLine(self, line: str):
        if len(line.strip()) == 0:
            return None

        if line[0] == '#':
            self.programID = int(line[1:].strip(),0)
            return None

        if line.strip().endswith('()'):
            funcName = line.strip()[:-len('()')]
            self.functions.update({
                funcName: len(self.bProgram)
            })
            return None

        valRes = self._validateLine(line)
        if valRes != True:
            return valRes

        splittedLine = re.split(' |, ', line)
        command = splittedLine[0].strip().lower()
        args = [x.strip() for x in splittedLine[1:]]

        bArray = bytearray()

        commandDef = self._instructionsDict.get(command)
        bCommandCode = bytes([commandDef.get("code")])
        bArray.extend(bCommandCode)

        argsCntDef = commandDef.get("arguments")
        argsTypesDef = commandDef.get("argumentTypes")
        for x in range(argsCntDef):
            bArgument = self._parseArgument(argsTypesDef[x], args[x])
            bArray.extend(bArgument)

        return bArray

    def _parseArgument(self, argType: str, arg: str):
        if argType == 'uint8':
            argInt = int(arg, 0)
            return argInt.to_bytes(1, byteorder='little')
        if argType == 'uint16':
            argInt = int(arg, 0)
            return argInt.to_bytes(2, byteorder='little')
        if argType == 'uint32':
            argInt = int(arg, 0)
            return argInt.to_bytes(4, byteorder='little')
        if argType == 'int8':
            argInt = int(arg, 0)
            return argInt.to_bytes(1, signed=True, byteorder='little')
        if argType == 'int16':
            argInt = int(arg, 0)
            return argInt.to_bytes(2, signed=True, byteorder='little')
        if argType == 'int32':
            argInt = int(arg, 0)
            return argInt.to_bytes(4, signed=True, byteorder='little')
        if argType == 'funcName':
            self.calls.update({
                len(self.bProgram) : arg
            })
            return int('0').to_bytes(2, byteorder='little')

    def _validateLine(self, line: str):
        if line.strip() == '':
            return None
        splittedLine = re.split(' |, ', line)
        if len(splittedLine) == 0:
            return False
        command = splittedLine[0].strip()
        args = splittedLine[1:]
        if command.lower() not in self._instructionsDict:
            print("ERROR! Command \"" + command + "\" not supported.")
            return False
        instructionDef = self._instructionsDict.get(command.lower())
        argCntDef = instructionDef.get("arguments")
        argCnt = len(args)
        if argCntDef != argCnt:
            print("ERROR! In command \"" + command +"\":")
            print("Wrong argument count. Actual: " + str(argCnt) + ", Expected: " + str(argCntDef))
            return False

        return True

    def _replaceCallsOffsets(self):
        for callOffset, funcName in self.calls.items():
            offset = self.functions.get(funcName)
            offset = offset.to_bytes(2, byteorder='little')
            _replaceInByteArray(self.bProgram, callOffset, offset)

def _replaceInByteArray(barr: bytearray, pos: int, src: bytearray):
    if len(barr) < len(src) + pos:
        raise Exception('bytearray to short')
    for ind, bt in enumerate(src):
        barr[pos + ind] = src[ind]
