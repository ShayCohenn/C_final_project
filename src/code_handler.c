#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/code_handler.h"
#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/errors.h"

int inc_memo(conv_code **code, int count) {
  conv_code *ptr;
  ptr = *code;
  
  *code = realloc(*code, (count + 1) * sizeof(conv_code));
  if(*code == NULL) {
    report_internal_error(ERROR_CODE_1);
    free(ptr);
    return 0;
  }
  return 1;
}

int add_machine_code_line(conv_code **code, unsigned short num, char *str, int *IC, location am_file) {
  if(inc_memo(code, *IC) == 0) return 0;
  
  (*code + *IC)->short_num = num;
  (*code + *IC)->line = am_file.line;
  
  if(str == NULL)
    (*code + *IC)->label = NULL;
  else {
    (*code + *IC)->label = handle_malloc((strlen(str)+1) * sizeof(char));
    if((*code + *IC)->label == NULL) return 0;
    
    strcpy((*code + *IC)->label, str);
  }
  return 1;
}

int add_machine_code_data(conv_code **data, inst_parts *inst, int *DC, location am_file) {
  int i, inst_len;
  inst_len = inst->len;
  if(data == NULL || *data == NULL) return 0;
  for(i = 0;i < inst_len; i++) {
    if(inc_memo(data, *DC) == 0) return 0;
    (*data + *DC)->short_num = *(inst->nums + i);
    (*data + *DC)->label = NULL;
    (*data + *DC)->line = am_file.line;
    (*DC)++;
  }
  return 1;
}
  
