#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#include <stdint.h>

#define VM_STACK_SIZE 128
#define VM_REGISTERS 10
#define VM_IO_DATA 4

typedef struct {
    uint32_t programCounter;
    uint32_t stackPointer;
    uint32_t stackArray[VM_STACK_SIZE];
} S_VMSystemRegisters;

typedef struct {
    uint32_t temp[VM_REGISTERS];
    uint32_t pers[VM_REGISTERS];
    uint32_t accA;
    uint32_t accB;
} S_VMRegisters;

typedef enum {
    INS_RET = 0x10,
    INS_CALL = 0x11,

    INS_JMP = 0x20,
    INS_JMPZ = 0x21,
    INS_JMPNZ = 0x22,

    INS_MOV = 0x30,

    INS_ADD = 0x40,
    INS_SUB = 0x41,
    INS_DIV = 0x42,
    INS_MOD = 0x43,
    INS_MUL = 0x44,
    INS_POW = 0x45
} E_Instructions;

typedef struct {
    uint32_t io[VM_IO_DATA];
} S_VMIOData;

typedef uint32_t PROGRAM_ID;

typedef struct {
    PROGRAM_ID id;
    uint32_t offset;
} S_VMProgram;

typedef struct {
    uint32_t programsCnt;
    S_VMProgram program[0];
} S_VMProgramsList;

typedef struct {
    uint32_t programsListOffset;
    uint32_t programsOffset;
} S_VMdata;

typedef struct {
    S_VMRegisters regs;
    S_VMSystemRegisters sysRegs;
    S_VMdata *VMData;
} S_VM;

#define VM_ERR_PREFIX 0x10000
#define VM_ERR_BUILDER(x) (VM_ERR_PREFIX + (x))

#define VM_ERR_SUCCESS 0
#define VM_ERR_DIVISION VM_ERR_BUILDER(0x01)
#define VM_ERR_OVERFLOW VM_ERR_BUILDER(0x02)
#define VM_ERR_PROG_NOT_FOUND VM_ERR_BUILDER(0x03)

typedef uint32_t ERR_STATUS;


ERR_STATUS VM_ENGINE(S_VM *vm,
                     PROGRAM_ID programID,
                     S_VMIOData *input,
                     S_VMIOData *output);

#endif // VM_H_INCLUDED
