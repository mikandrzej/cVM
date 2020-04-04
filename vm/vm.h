#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#include <stdint.h>

#include "common/utils.h"
#include "common/errors.h"

#define VM_STACK_SIZE 128
#define VM_REGISTERS 10
#define VM_IO_DATA 4

#pragma pack(push)
#pragma pack(1)
typedef struct {
    uint32_t programCounter;
    uint32_t stackPointer;
    uint32_t stackArray[VM_STACK_SIZE];
} S_VMSystemRegisters;

typedef struct {
    uint32_t temp[VM_REGISTERS];
    uint32_t pers[VM_REGISTERS];
} S_VMRegisters;


#define INS_RET 0x10
#define INS_CALL 0x11

#define INS_JMP 0x20
#define INS_JMPZ 0x21
#define INS_JMPNZ 0x22

#define INS_MOV 0x30

#define INS_ADD 0x40
#define INS_SUB 0x41
#define INS_DIV 0x42
#define INS_MOD 0x43
#define INS_MUL 0x44
#define INS_POW 0x45

#define INS_PRINT_REGS 0x50

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

typedef struct
{
    uint8_t a;
    uint8_t b;
} S_InsMath2Ops;

typedef struct {
    uint8_t regA;
    uint8_t regB;
    int32_t cnst;
} S_InsMath3Ops;

typedef union{
    S_InsMath1Op math1Op;
    S_InsMath2Ops math2Ops;
    S_InsMath3Ops math3Ops;
} U_InsArgs;

typedef struct {
    uint8_t code;
    U_InsArgs args;
} S_VMInstruction;

#pragma pack(pop)
ERR_STATUS VM_ENGINE(S_VM *vm,
                     PROGRAM_ID programID,
                     S_VMIOData *input,
                     S_VMIOData *output);

S_VM* VM_Init();

#define BREAK_AND_SET_ERROR(var, code) {var = code; break;}

#endif // VM_H_INCLUDED
