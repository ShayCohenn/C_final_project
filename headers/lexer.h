#ifndef LEXER_H
#define LEXER_H

typedef struct opcode {
  char *opcode;
  int arg_num;
} opcode;

#define OPCODES_COUNT 16
extern opcode OPCODE[];

typedef struct command_parts {
  char *label;
  int opcode;
  char *source;
  char *dest;
} command_parts;

typedef struct inst_parts {
  char *label;
  short *nums;
  int len;
  char *arg_label;
  int is_extern;
} inst_parts;

#define INSTRUCTION_COUNT 5

#define REG_COUNT 8

#endif
