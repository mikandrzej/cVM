#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

static ERR_STATUS VM_InstructionAdd(S_InsMath3Ops *arguments, S_VMRegisters *registers)
{
    if (arguments->regA >= VM_REGISTERS)
    {
        return VM_ERR_INS_ARG_OUT_OF_RANGE;
    }

    if (arguments->regB != 0xFF)
    { //a regB to regA
        if (arguments->regB >= VM_REGISTERS)
        {
            return VM_ERR_INS_ARG_OUT_OF_RANGE;
        }
        *((int32_t *)&registers->temp[arguments->regA]) += *((int32_t *)&registers->temp[arguments->regB]);
    }
    else
    { //const value to regA
        *((int32_t *)&registers->temp[arguments->regA]) += arguments->cnst;
    }

    return VM_ERR_SUCCESS;
}

static ERR_STATUS VM_InstructionSub(S_InsMath3Ops *arguments, S_VMRegisters *registers)
{
    if (arguments->regA >= VM_REGISTERS)
    {
        return VM_ERR_INS_ARG_OUT_OF_RANGE;
    }

    if (arguments->regB != 0xFF)
    { //regB to regA
        if (arguments->regB >= VM_REGISTERS)
        {
            return VM_ERR_INS_ARG_OUT_OF_RANGE;
        }
        *((int32_t *)&registers->temp[arguments->regA]) -= *((int32_t *)&registers->temp[arguments->regB]);
    }
    else
    { //const value to regA
        *((int32_t *)&registers->temp[arguments->regA]) -= arguments->cnst;
    }

    return VM_ERR_SUCCESS;
}

static ERR_STATUS VM_InstructionMul(S_InsMath3Ops *arguments, S_VMRegisters *registers)
{
    if (arguments->regA >= VM_REGISTERS)
    {
        return VM_ERR_INS_ARG_OUT_OF_RANGE;
    }

    if (arguments->regB != 0xFF)
    { //regB to regA
        if (arguments->regB >= VM_REGISTERS)
        {
            return VM_ERR_INS_ARG_OUT_OF_RANGE;
        }
        *((int32_t *)&registers->temp[arguments->regA]) *= *((int32_t *)&registers->temp[arguments->regB]);
    }
    else
    { //const value to regA
        *((int32_t *)&registers->temp[arguments->regA]) *= arguments->cnst;
    }

    return VM_ERR_SUCCESS;
}

static ERR_STATUS VM_InstructionDiv(S_InsMath3Ops *arguments, S_VMRegisters *registers)
{
    if (arguments->regA >= VM_REGISTERS)
    {
        return VM_ERR_INS_ARG_OUT_OF_RANGE;
    }

    if (arguments->regB != 0xFF)
    { //regB to regA
        if (arguments->regB >= VM_REGISTERS)
        {
            return VM_ERR_INS_ARG_OUT_OF_RANGE;
        }
        if (registers->temp[arguments->regB] == 0)
        {
            return VM_ERR_MATH_DIV_BY_0;
        }
        *((int32_t *)&registers->temp[arguments->regA]) /= *((int32_t *)&registers->temp[arguments->regB]);
    }
    else
    { //const value to regA
        if (arguments->cnst == 0)
        {
            return VM_ERR_MATH_DIV_BY_0;
        }
        *((int32_t *)&registers->temp[arguments->regA]) /= arguments->cnst;
    }

    return VM_ERR_SUCCESS;
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

    uint16_t pc = 0;
    ERR_STATUS vmErrCode = VM_ERR_SUCCESS;
    while(1)
    {
        S_VMInstruction *instruction = (S_VMInstruction*) (program + pc);

        switch(instruction->code)
        {
        case INS_ADD:
            vmErrCode = VM_InstructionAdd(&instruction->args.math3Ops, &vm->regs);
            pc += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            break;

        case INS_SUB:
            vmErrCode = VM_InstructionSub(&instruction->args.math3Ops, &vm->regs);
            pc += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            break;

        case INS_MUL:
            vmErrCode = VM_InstructionMul(&instruction->args.math3Ops, &vm->regs);
            pc += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
            break;

        case INS_DIV:
            vmErrCode = VM_InstructionDiv(&instruction->args.math3Ops, &vm->regs);
            pc += (uint16_t)(sizeof(instruction->code) + sizeof(S_InsMath3Ops));
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
        printf("VM fail on pc: %u", pc);
        return vmErrCode;
    }

    return VM_ERR_SUCCESS;
}
