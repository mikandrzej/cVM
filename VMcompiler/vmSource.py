from vmProgram import VMProgram

class VMSource(object):
    def __init__(self, inputPaths: list, outputPath: str):
        self.inputPaths = inputPaths
        self.outputPath = outputPath
        self.bOutput = bytearray()
        self.programs = []
        self._parsePrograms()
        self._prepareOutput()
        self.saveOutput()

    def _parsePrograms(self):
        programs = []
        for path in self.inputPaths:
            with open(path) as f:
                fileText = f.read()
                programs.append(VMProgram(fileText))

        self.programs = programs

    def _prepareOutput(self):
        actOffset = 0

        actOffset += 2
        programListOffset = actOffset

        programsList = []
        actOffset += 2
        programsCnt = len(self.programs)

        bOutput = bytearray()
        bOutput.extend(programListOffset.to_bytes(2, byteorder='little'))
        bOutput.extend(programsCnt.to_bytes(2, byteorder='little'))

        actOffset += programsCnt * 4

        for program in self.programs:
            programEntry = {}
            programEntry['ID'] = program.programID
            programEntry['offset'] = actOffset
            programEntry['program'] = program
            programsList.append(programEntry)
            actOffset += program.programLen

        for program in programsList:
            bOutput.extend(program['ID'].to_bytes(2, byteorder='little'))
            bOutput.extend((program['offset']).to_bytes(2, byteorder='little'))

        for program in programsList:
            bOutput.extend(program['program'].bProgram)

        self.bOutput = bOutput

    def saveOutput(self):
        with open(self.outputPath, "wb") as f:
            f.write(self.bOutput)