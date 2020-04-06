#include "vm.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

S_VM* VM_Init()
{
    S_VM *vm = (S_VM*) calloc(sizeof(S_VM), 1);
    return vm;
}

ERR_STATUS VM_FindProgram(PROGRAM_ID id, S_VM *vm, uint16_t *programOffset)
{
    if(vm == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    
    ERR_STATUS retval = VM_ERR_PROG_NOT_FOUND;
    uint8_t *basePointer = (uint8_t *)(vm->VMData);

    S_VMProgramsList *VMProgramsList = (S_VMProgramsList *)(basePointer + vm->VMData->programsListOffset);

    for(int k=0; k<VMProgramsList->programsCnt; k++)
    {
        if(VMProgramsList->program[k].id == id)
        {
            *programOffset = VMProgramsList->program[k].offset;
            retval = VM_ERR_SUCCESS;
            break;
        }
    }

    return retval;
}

ERR_STATUS VM_ENGINE(S_VM *vm,
                     PROGRAM_ID programID,
                     S_VMIOData *input,
                     S_VMIOData *output)
{
    if(vm == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }

    ERR_STATUS retval = VM_ERR_SUCCESS;
    S_VMProgramsList *VMProgramsList= nullptr;
    uint16_t programOffset = 0;
    uint8_t *basePointer = (uint8_t*)vm->VMData;
    uint8_t *program = nullptr;

    retval = VM_FindProgram(programID, vm, &programOffset);
    RETURN_ON_ERROR(retval);

    program = (uint8_t *) (programOffset + basePointer);

    vm->sysRegs.programCounter = 0;
    vm->sysRegs.stack.pointer = 0;

    if(input != nullptr)
    {
        for(int k=0; k<VM_IO_DATA; k++)
        {
            vm->regs.temp[k] = input->io[k];
        }
    }

    ERR_STATUS vmErrCode = VM_ERR_SUCCESS;
    while(1)
    {
        S_VMInstruction *instruction = (S_VMInstruction*) (program + vm->sysRegs.programCounter);

        switch(instruction->code)
        {
        case INS_ADD:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            vmErrCode = VM_InstructionAdd(&instruction->args.math3Ops, &vm->regs);
            break;

        case INS_SUB:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            vmErrCode = VM_InstructionSub(&instruction->args.math3Ops, &vm->regs);
            break;

        case INS_MUL:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            vmErrCode = VM_InstructionMul(&instruction->args.math3Ops, &vm->regs);
            break;

        case INS_DIV:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            vmErrCode = VM_InstructionDiv(&instruction->args.math3Ops, &vm->regs);
            break;

        case INS_MOD:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            vmErrCode = VM_InstructionMod(&instruction->args.math3Ops, &vm->regs);
            break;

        case INS_MOV:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            vmErrCode = VM_InstructionMov(&instruction->args.math3Ops, &vm->regs);
            break;

        case INS_PUSH:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsReg));
            vmErrCode = VM_InstructionPush(&vm->sysRegs.stack, &vm->regs,  &instruction->args.reg);
            break;

        case INS_POP:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsReg));
            vmErrCode = VM_InstructionPop(&vm->sysRegs.stack, &vm->regs,  &instruction->args.reg);
            break;

        case INS_JMP:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsProgCnt));
            vmErrCode = VM_InstructionJmp(&vm->sysRegs.programCounter, 
                                            &vm->sysRegs.heap, 
                                            &instruction->args.programCounter);
            break;

        case INS_RET:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code));
            vmErrCode = VM_InstructionRet(&vm->sysRegs.programCounter, &vm->sysRegs.heap);
            break;

        case INS_CALL:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsCall));
            vmErrCode = VM_InstructionCall(&vm->sysRegs.programCounter,
                                            &vm->sysRegs.heap, 
                                            &instruction->args.call);
            break;

        case INS_PRINT_REGS:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code));
            vmErrCode = VM_InstructionPrintRegs(&vm->regs);
            break;

        default:
            vmErrCode = VM_ERR_INS_NOT_FOUND;
        }

        if(vmErrCode)
        {
            break;
        }
    }

    if (vmErrCode == VM_ERR_APP_END)
    {
        if (output != nullptr)
        {
            for(int k=0; k<VM_IO_DATA; k++)
            {
                output->io[k] = vm->regs.temp[k];
            }
        }
        return VM_ERR_SUCCESS;
    }
    else if (vmErrCode != VM_ERR_SUCCESS)
    {
        printf("VM fail on pc: %u \r\n", vm->sysRegs.programCounter);
        return vmErrCode;
    }

    return VM_ERR_UNKNOWN;
}

