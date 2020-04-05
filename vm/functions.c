#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

ERR_STATUS VM_InstructionAdd(S_InsMath3Ops *arguments, S_VMRegisters *registers)
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

ERR_STATUS VM_InstructionSub(S_InsMath3Ops *arguments, S_VMRegisters *registers)
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

ERR_STATUS VM_InstructionMul(S_InsMath3Ops *arguments, S_VMRegisters *registers)
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

ERR_STATUS VM_InstructionDiv(S_InsMath3Ops *arguments, S_VMRegisters *registers)
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

ERR_STATUS VM_InstructionMod(S_InsMath3Ops *arguments, S_VMRegisters *registers)
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
        *((int32_t *)&registers->temp[arguments->regA]) %= *((int32_t *)&registers->temp[arguments->regB]);
    }
    else
    { //const value to regA
        if (arguments->cnst == 0)
        {
            return VM_ERR_MATH_DIV_BY_0;
        }
        *((int32_t *)&registers->temp[arguments->regA]) %= arguments->cnst;
    }

    return VM_ERR_SUCCESS;
}

ERR_STATUS VM_InstructionPrintRegs(S_VMRegisters *registers)
{
    if(registers == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }

    printf("Registers:\r\n");

    printf(" TEMPORARY\r\n");
    for(int k=0; k<VM_REGISTERS; k++)
    {
        printf("  0x%02x: 0x%08x\r\n", k, registers->temp[k]);
    }

    printf("\r\n"
           "PERSISTENT\r\n");
    for(int k=0; k<VM_REGISTERS; k++)
    {
        printf("  0x%02x: 0x%08x\r\n", k, registers->pers[k]);
    }
    return VM_ERR_SUCCESS;
}

ERR_STATUS VM_InstructionPush(S_VMStack *stack, S_VMRegisters *registers, S_InsReg *reg)
{
    if(stack == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    if(reg == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    if(reg->regNo >= VM_REGISTERS)
    {
        return VM_ERR_INS_ARG_OUT_OF_RANGE;
    }

    stack->stack[stack->pointer] = registers->temp[reg->regNo];
    stack->pointer++;

    if(stack->pointer >= VM_STACK_SIZE || stack->pointer == 0)
    {
        return VM_ERR_STACK_OVERFLOW;
    }
    return VM_ERR_SUCCESS;
}

ERR_STATUS VM_InstructionPop(S_VMStack *stack, S_VMRegisters *registers, S_InsReg *reg)
{
    if(stack == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    if(reg == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    if(reg->regNo >= VM_REGISTERS)
    {
        return VM_ERR_INS_ARG_OUT_OF_RANGE;
    }

    if(stack->pointer < (stack->pointer-1))
    {
        return VM_ERR_STACK_OVERFLOW;
    }

    stack->pointer--;
    registers->temp[reg->regNo] = stack->stack[stack->pointer];

    return VM_ERR_SUCCESS;
}

ERR_STATUS VM_InstructionMov(S_InsMath3Ops *arguments, S_VMRegisters *registers)
{
    if(registers == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    if(arguments == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
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
        *((int32_t *)&registers->temp[arguments->regA]) = *((int32_t *)&registers->temp[arguments->regB]);
    }
    else
    { //const value to regA
        *((int32_t *)&registers->temp[arguments->regA]) = arguments->cnst;
    }

    return VM_ERR_SUCCESS;
}

ERR_STATUS VM_InstructionCall(uint32_t *actProgramCounter, S_VMHeap *heap, S_InsProgCnt *arguments)
{
    if(actProgramCounter == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    if(heap == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    if(arguments == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    if(heap->pointer+1 >= VM_HEAPSIZE)
    {
        return VM_ERR_HEAP_OVERFLOW;
    }
    if(heap->pointer+1 < heap->pointer)
    {
        return VM_ERR_HEAP_OVERFLOW;
    }

    heap->callCtx[heap->pointer].programCounter = *actProgramCounter;
    *actProgramCounter = arguments->programCounter;

    return VM_ERR_SUCCESS;
}

ERR_STATUS VM_InstructionRet(uint32_t *actProgramCounter, S_VMHeap *heap)
{
    if(actProgramCounter == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    if(heap == nullptr)
    {
        return VM_ERR_INVALID_PTR;
    }
    if(heap->pointer < heap->pointer-1)
    {
        return VM_ERR_OVERFLOW;
    }

    heap->pointer--;
    *actProgramCounter = heap->callCtx[heap->pointer].programCounter;

    return VM_ERR_SUCCESS;
}