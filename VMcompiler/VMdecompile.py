import os
import sys


class VMDecompiler(object):
    from vmInstructions import instructionsDict
    def __init__(self, inputFilePath: str, outputPath: str):
        self.inputFilePath = inputFilePath
        self.outputPath = outputPath
        self.bInputFile = bytearray()

        self._loadInputFile()
        self._parseProgramHeader()
        self._saveOutput()

    def _loadInputFile(self):
        with open(self.inputFilePath, "rb") as f:
            self.bInputFile = f.read()

    def _parseProgramHeader(self):
        header = {}
        programsListOffset = int.from_bytes(self.bInputFile[0:2],
                                            byteorder='little',
                                            signed=False)
        header['programsListOffset'] = programsListOffset

        programsCnt = int.from_bytes(self.bInputFile[programsListOffset:programsListOffset+2],
                                     byteorder='little',
                                     signed=False)
        programs = {}
        for k in range(programsCnt):
            programIDOffset = programsListOffset + 2 + (k*4)
            programOffset = programsListOffset + 4 + (k*4)
            programID = int.from_bytes(self.bInputFile[programIDOffset:programIDOffset+2],
                                       byteorder='little',
                                       signed=False)
            programStartOffset = int.from_bytes(self.bInputFile[programOffset:programOffset+2],
                                       byteorder='little',
                                       signed=False)
            program = self._parsePrograms(self.bInputFile[programStartOffset:])

            programs[programID] = program

        self._parsedPrograms = programs

    def _parsePrograms(self, program: bytearray()):
        callLevel = 0
        actOffset = 0
        programLines = []
        while True:
            commandCode = int.from_bytes(program[actOffset:actOffset+1],
                                     byteorder='little',
                                     signed=False)
            instructionOffset = actOffset
            actOffset += 1
            instructionName = None
            instruction = None
            for _instructionName, value in self.instructionsDict.items():
                instructionCode = value.get('code')
                if instructionCode == commandCode:
                    instruction = value
                    instructionName = _instructionName
                    break

            if instruction == None:
                raise Exception('Instruction with code ' + str(instructionCode) + ' not found!')


            arguments = []
            for argType in instruction.get('argumentTypes'):
                (arg, argLen) = self._readArgument(program[actOffset:],
                                                argType)
                actOffset += argLen
                arguments.append(arg)

            programLine = "0x%0.4X: " %instructionOffset
            programLine += instructionName + ' '
            for ind, arg in enumerate(arguments):
                if ind != 0:
                    programLine += ', '
                programLine += "0x%X" % arg

            programLines.append(programLine)

            if instructionName == 'call':
                callLevel += 1
            elif instructionName == 'ret':
                callLevel -= 1

            if callLevel < 0:
                break

            if actOffset > len(program):
                raise Exception('program doesn\'t return!')
        return programLines

    def _readArgument(self, program: bytearray(), argType: str):
        if argType == 'uint8':
            return (int.from_bytes(program[:1],
                                   byteorder='little',
                                   signed=False),
                    1)
        if argType == 'uint16':
            return (int.from_bytes(program[:2],
                                   byteorder='little',
                                   signed=False),
                    2)
        if argType == 'uint32':
            return (int.from_bytes(program[:4],
                                   byteorder='little',
                                   signed=False),
                    4)
        if argType == 'int8':
            return (int.from_bytes(program[:1],
                                   byteorder='little',
                                   signed=True),
                    1)
        if argType == 'int16':
            return (int.from_bytes(program[:2],
                                   byteorder='little',
                                   signed=True),
                    2)
        if argType == 'int32':
            return (int.from_bytes(program[:4],
                                   byteorder='little',
                                   signed=True),
                    4)
        if argType == 'funcName':
            return (int.from_bytes(program[:4],
                                   byteorder='little',
                                   signed=False),
                    4)

    def _saveOutput(self):
        for programID, programLines in self._parsedPrograms.items():
            fileName = 'decompiled_' + str(programID) + '.vms'
            with open(os.path.join(self.outputPath, fileName), 'w') as f:
                for line in programLines:
                    f.write(line + '\r')
                f.close()


if __name__ == '__main__':
    VMDecompiler('C:\\Miki\\priv\\Repos\\cVM\\VMcompiler\\compiled.bin', 'C:\\Miki\\priv\\Repos\\cVM\\VMcompiler\\decomp')