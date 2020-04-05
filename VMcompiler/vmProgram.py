import re

class VMProgram(object):
    from VMcompiler.vmInstructions import instructionsDict as _instructionsDict

    def __init__(self, text: str):
        self.bProgram = bytearray()
        self.programID = 0xFFFF
        self.programLen = 0
        self.importFromText(text)
        pass

    def importFromText(self, text: str):
        bProgram = bytearray()

        lines = text.splitlines(keepends=False)
        for line in lines:
            parseRes = self._parseLine(line)
            if parseRes == None:
                continue
            elif parseRes == False:
                raise Exception("Compilation failed.")

            if type(parseRes) is bytearray:
                bProgram.extend(parseRes)

        self.bProgram = bProgram
        self.programLen = len(bProgram)


    def _parseLine(self, line: str):
        if len(line.strip()) == 0:
            return None

        if line[0] == '#':
            self.programID = int(line[1:].strip(),0)
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
        argInt = int(arg,0)
        if argType == 'uint8':
            return argInt.to_bytes(1, byteorder='little')
        if argType == 'uint16':
            return argInt.to_bytes(2, byteorder='little')
        if argType == 'uint32':
            return argInt.to_bytes(4, byteorder='little')
        if argType == 'int8':
            return argInt.to_bytes(1, signed=True, byteorder='little')
        if argType == 'int16':
            return argInt.to_bytes(2, signed=True, byteorder='little')
        if argType == 'int32':
            return argInt.to_bytes(4, signed=True, byteorder='little')

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


