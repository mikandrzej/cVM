
#include "common/utils.h"
#include "common/errors.h"
#include "vm.h"

#define INS_RET 0x10
#define INS_CALL 0x11
#define INS_PUSH 0x12
#define INS_POP 0x13
#define INS_RUN 0x14

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
ERR_STATUS VM_InstructionAdd(S_InsMath3Ops *arguments, S_VMRegisters *registers);
ERR_STATUS VM_InstructionSub(S_InsMath3Ops *arguments, S_VMRegisters *registers);
ERR_STATUS VM_InstructionMul(S_InsMath3Ops *arguments, S_VMRegisters *registers);
ERR_STATUS VM_InstructionDiv(S_InsMath3Ops *arguments, S_VMRegisters *registers);
ERR_STATUS VM_InstructionMod(S_InsMath3Ops *arguments, S_VMRegisters *registers);


ERR_STATUS VM_InstructionPush(S_VMStack *stack, S_VMRegisters *registers, S_InsReg *reg);
ERR_STATUS VM_InstructionPop(S_VMStack *stack, S_VMRegisters *registers, S_InsReg *reg);
ERR_STATUS VM_InstructionMov(S_InsMath3Ops *arguments, S_VMRegisters *registers);


ERR_STATUS VM_InstructionJmp(uint32_t *actProgramCounter, S_VMHeap *heap, S_InsProgCnt *arguments);
ERR_STATUS VM_InstructionRet(uint32_t *actProgramCounter, S_VMHeap *heap);
ERR_STATUS VM_InstructionCall(uint32_t *actProgramCounter, S_VMHeap *heap, S_InsCall *arguments);

ERR_STATUS VM_InstructionPrintRegs(S_VMRegisters *registers);