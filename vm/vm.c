#include "vm.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

S_VM* VM_Init()
{
    S_VM *vm = (S_VM*) calloc(sizeof(S_VM), 1);
    return vm;
}

ERR_STATUS VM_ENGINE(S_VM *vm,
                     PROGRAM_ID programID,
                     S_VMIOData *input,
                     S_VMIOData *output)
{
    uint8_t *basePointer = (uint8_t*)vm->VMData;
    S_VMProgramsList *VMProgramsList= nullptr;
    S_VMProgram *VMProgram = nullptr;
    uint8_t *program = nullptr;

    VMProgramsList = (S_VMProgramsList *)(basePointer + vm->VMData->programsListOffset);
    for(int k=0; k<VMProgramsList->programsCnt; k++)
    {
        if(VMProgramsList->program[k].id == programID)
        {
            program = (uint8_t *)(basePointer + VMProgramsList->program[k].offset);
            break;
        }
    }

    if (program == nullptr)
    {
        return VM_ERR_PROG_NOT_FOUND;
    }

    vm->sysRegs.programCounter = 0;
    vm->sysRegs.stack.pointer = 0;

    ERR_STATUS vmErrCode = VM_ERR_SUCCESS;
    while(1)
    {
        S_VMInstruction *instruction = (S_VMInstruction*) (program + vm->sysRegs.programCounter);

        switch(instruction->code)
        {
        case INS_ADD:
            vmErrCode = VM_InstructionAdd(&instruction->args.math3Ops, &vm->regs);
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            break;

        case INS_SUB:
            vmErrCode = VM_InstructionSub(&instruction->args.math3Ops, &vm->regs);
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            break;

        case INS_MUL:
            vmErrCode = VM_InstructionMul(&instruction->args.math3Ops, &vm->regs);
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            break;

        case INS_DIV:
            vmErrCode = VM_InstructionDiv(&instruction->args.math3Ops, &vm->regs);
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            break;

        case INS_MOD:
            vmErrCode = VM_InstructionMod(&instruction->args.math3Ops, &vm->regs);
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            break;

        case INS_MOV:
            vmErrCode = VM_InstructionMov(&instruction->args.math3Ops, &vm->regs);
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            break;

        case INS_PUSH:
            vmErrCode = VM_InstructionPush(&vm->sysRegs.stack, &vm->regs,  &instruction->args.reg);
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsReg));
            break;

        case INS_POP:
            vmErrCode = VM_InstructionPop(&vm->sysRegs.stack, &vm->regs,  &instruction->args.reg);
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsReg));
            break;

        case INS_CALL:
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsProgCnt));
            vmErrCode = VM_InstructionCall(&vm->sysRegs.programCounter, 
                                            &vm->sysRegs.heap, 
                                            &instruction->args.programCounter);
            break;

        case INS_RET:
            vmErrCode = VM_InstructionRet(&vm->sysRegs.programCounter, &vm->sysRegs.heap);
            break;

        case INS_PRINT_REGS:
            vmErrCode = VM_InstructionPrintRegs(&vm->regs);
            vm->sysRegs.programCounter += (uint16_t)(sizeof(instruction->code));
            break;

        default:
            vmErrCode = VM_ERR_INS_NOT_FOUND;
        }

        if(vmErrCode)
        {
            break;
        }
    }
    
    if(vmErrCode)
    {
        printf("VM fail on pc: %u", vm->sysRegs.programCounter);
        return vmErrCode;
    }

    return VM_ERR_SUCCESS;
}

