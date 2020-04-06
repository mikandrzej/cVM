#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#include <stdint.h>

#include "common/utils.h"
#include "common/errors.h"

#define VM_STACK_SIZE 128
#define VM_HEAPSIZE 128
#define VM_REGISTERS 10
#define VM_IO_DATA 4

#pragma pack(push)
#pragma pack(1)

typedef struct {
    uint8_t pointer;
    uint32_t stack[VM_STACK_SIZE];
} S_VMStack;

typedef struct {
    uint32_t programCounter; 
} S_CallCtx;

typedef struct {
    uint8_t pointer;
    S_CallCtx callCtx[VM_HEAPSIZE];
} S_VMHeap;

typedef struct {
    uint32_t programCounter;
    S_VMStack stack;
    S_VMHeap heap;
} S_VMSystemRegisters;

typedef struct {
    uint32_t temp[VM_REGISTERS];
    uint32_t pers[VM_REGISTERS];
} S_VMRegisters;

typedef struct{
    uint32_t io[VM_IO_DATA];
} S_VMIOData;

typedef uint16_t PROGRAM_ID;

typedef struct {
    PROGRAM_ID id;
    uint16_t offset;
} S_VMProgram;

typedef struct {
    uint16_t programsCnt;
    S_VMProgram program[0];
} S_VMProgramsList;

typedef struct {
    uint16_t programsListOffset;
} S_VMdata;

typedef struct {
    S_VMRegisters regs;
    S_VMSystemRegisters sysRegs;
    S_VMdata *VMData;
} S_VM;

typedef struct
{
    uint8_t a;
} S_InsMath1Op;

typedef struct{
    uint8_t a;
    uint8_t b;
} S_InsMath2Ops;

typedef struct {
    uint8_t regA;
    uint8_t regB;
    int32_t cnst;
} S_InsMath3Ops;

typedef struct {
    uint8_t regNo;
} S_InsReg;

typedef struct {
    uint16_t pc;
} S_InsCall;

typedef struct {
    uint8_t programCounter;
} S_InsProgCnt;

typedef union{
    S_InsMath1Op math1Op;
    S_InsMath2Ops math2Ops;
    S_InsMath3Ops math3Ops;
    S_InsReg reg;
    S_InsProgCnt programCounter;
    S_InsCall call;
} U_InsArgs;

typedef struct {
    uint8_t code;
    U_InsArgs args;
} S_VMInstruction;


#pragma pack(pop)

ERR_STATUS VM_FindProgram(PROGRAM_ID id, S_VM *vm, uint16_t *programOffset);
ERR_STATUS VM_ENGINE(S_VM *vm,
                     PROGRAM_ID programID,
                     S_VMIOData *input,
                     S_VMIOData *output);

S_VM* VM_Init();

#define BREAK_AND_SET_ERROR(var, code) {var = code; break;}

#endif // VM_H_INCLUDED
