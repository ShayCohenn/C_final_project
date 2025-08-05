#include <stdio.h>
#include "../headers/lexer.h"

char *REGS[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
char *INSTRUCTIONS[] = {".data", ".string", ".mat", ".entry", ".extern"};
/*continue opcodes list*/
opcode OPCODES[] = {
  {"mov", 2},
  {"cmp", 2},
  {"add", 2},
  {"sub", 2},
  {"not", 1},
  {"mov", 2},
  {"mov", 2},
  {"mov", 2},
  {"mov", 2},
  {"mov", 2},
  {"mov", 2},
  {"mov", 2},
  {"mov", 2},
  {"mov", 2},
  {"mov", 2},
  {"mov", 2}
};
                  
