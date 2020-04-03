#include "vm.h"

ERR_STATUS VM_ENGINE(S_VM *vm,
                     PROGRAM_ID programID,
                     S_VMIOData *input,
                     S_VMIOData *output)
{
    void *basePointer = (void*)vm->VMData;
    S_VMProgramsList *VMProgramsList= 0;
    S_VMProgram *VMProgram = 0;
    VMProgramsList = (S_VMProgramsList*) (basePointer + vm->VMData->programsListOffset);
    for(int k=0; k<VMProgramsList->programsCnt; k++)
    {
        if(VMProgramsList->program[k].id == programID)
        {
            VMProgram = (S_VMProgram*) (basePointer + VMProgramsList->program[k].offset);
            break;
        }
    }

    if(VMProgram == 0)
    {
        return VM_ERR_PROG_NOT_FOUND;
    }

    return VM_ERR_SUCCESS;
}
