#ifndef CODE_HANDLER_H
#define CODE_HANDLER_H

#include "globals.h"
#include "lexer.h"

typedef struct conv_code {
  char *label;
  int line;
  unsigned short short_num;
} conv_code;

int inc_memo(conv_code **code, int count);
int add_machine_code_line(conv_code **code, unsigned short num, char *str, int *IC, location am_file);
int add_machine_code_data(conv_code **data, inst_parts *inst, int *DC, location am_file);
int add_extra_machine_code_line(conv_code **code, command_parts *command, location am_file, int *IC, int is_src); 
short command_to_binary(command_parts *command);
unsigned short reg_to_binary(command_parts *command, int reg_src);
int join_data_code(conv_code **code, conv_code *data, int IC, int DC);

#endif
