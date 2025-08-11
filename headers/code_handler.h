#ifndef CODE_HANDLER_H
#define CODE_HANDLER_H

#include "globals.h"

typedef struct conv_code {
  char *label;
  int line;
  unsigned short short_num;
} conv_code;

int inc_memo(conv_code **code, int count);
int add_machine_code_line(conv_code **code, unsigned short num, char *str, int *IC, location am_file);
int add_machine_code_data(conv_code **data, inst_parts *inst, int *DC, location am_file);

#endif
