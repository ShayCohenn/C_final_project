#include <stdio.h>
#include <string.h>
#include "../headers/lexer.h"

char *REGS[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
char *INSTRUCTIONS[] = {".data", ".string", ".mat", ".entry", ".extern"};
/*continue opcodes list*/
opcode OPCODES[] = {
  {"mov", 2},{"cmp", 2},{"add", 2},{"sub", 2},
  {"not", 1},{"clr", 1},{"lea", 2},{"inc", 1},
  {"dec", 1},{"jmp", 1},{"bne", 1},{"red", 1},
  {"prn", 1},{"jsr", 1},{"rts", 0},{"stop", 0}
};

int opcode_index(char *str) {
  int i;
  if(str == NULL) return -1;
  
  for(i = 0;i < OPCODES_COUNT;i++) {
    if(strcmp(str, OPCODE[i].opcode) == 0)
      return i;
  }
  return -1;
}

int is_instruction(char *str) {
  int i;
  if(str == NULL) return 0;
  for(i = 0; i < INSTRUCTION_COUNT; i++) {
    if(strcmp(str, INSTRUCTIONS[i]) == 0)
      return 1;
  }
  return 0;
}

int reg_index(char *str) {
  int i;
  if(str == NULL) return -1;
  for(i = 0; i < REG_COUNT; i++) {
    if(strcmp(str, REGS[i]) == 0)
      return i;
  }
  return -1;
}
  
