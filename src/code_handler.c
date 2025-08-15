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

unsigned short reg_to_binary(command_parts *command, int reg_src) {
  static int done;
  int reg1, reg2;
  unsigned short reg_src_num, reg_dest_num;
  reg_src_num = 0;
  reg_dest_num = 0;
  if(reg_src) {
    if((reg1 = reg_index(command->source)) >= 0)
      reg_src_num = reg1 << REG_SRC_BITS_SHIFT;
    if((reg2 = reg_index(command->dest)) >= 0)
      reg_dest_num = reg2 << REG_DEST_BITS_SHIFT;
    done = 1;
    return combine_bits(reg_src_num, reg_dest_num, 0);
  }
  else if(done == 0) {
    if((reg2 = reg_index(command->dest)) >= 0)
      reg_dest_num = reg2 << REG_DEST_BITS_SHIFT;
    return reg_dest_num;
  }
  done = 0;
  return -1;
}

int add_extra_machine_code_line(conv_code **code, command_parts *command, location am_file, int *IC, int is_src) {
  unsigned short num;
  char *arg;
  
  arg = (is_src) ? command->source : command->dest;
  if(reg_index(arg) > 0 && (num = reg_to_binary(command, is_src)) != -1) {
    (*IC)++;
    if(add_machine_code_line(code, num, NULL, IC, am_file) == 0) return 0;
  }
  else if(valid_label(arg)) {
    (*IC)++;
    if(add_machine_code_line(code, 2, arg, IC, am_file) == 0) return 0;
  }
  else if(is_int(arg)) {
    (*IC)++;
    if(add_machine_code_line(code, atoi(arg) << ARE_BITS, NULL, IC, am_file) == 0) return 0;
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

short command_to_binary(command_parts *command) {
  unsigned short src, opc, dest;
  src = 0;
  opc = 0;
  dest = 0;
  
  if(is_int(command->source))
    src = (short)(DIRECT_ADDRESSING << SRC_BITS_SHIFT);
  else if(valid_label(command->source))
    src = (short)(LABEL_ADDRESSING << SRC_BITS_SHIFT);
  else if(valid_mat(command->source))
    src = (short)(MAT_ADDRESSING << SRC_BITS_SHIFT);
  else if(reg_index(command->source) >= 0)
    src = (short)(REG_ADDRESSING << SRC_BITS_SHIFT);
  
  if(is_int(command->dest))
    dest = (short)(DIRECT_ADDRESSING << DEST_BITS_SHIFT);
  else if(valid_label(command->dest))
    dest = (short)(LABEL_ADDRESSING << DEST_BITS_SHIFT);
  else if(valid_mat(command->dest))
    dest = (short)(MAT_ADDRESSING << DEST_BITS_SHIFT);
  else if(reg_index(command->dest))
    dest = (short)(REG_ADDRESSING << DEST_BITS_SHIFT);
    
  opc = (short)((command->opcode) << OPCODE_BITS_SHIFT);
  
  return combine_bits(opc, src, dest);
}

int join_data_code(conv_code **code, conv_code *data, int IC, int DC) {
  int i;
  conv_code *ptr;
  ptr = *code;
  
  if(inc_memo(code, IC+DC) == 0) { free(ptr); free(data); return 0; }
  
  for(i = 0; i < DC; i++) {
    (*code + IC + i + 1)->label = (data + i)->label;
    (*code + IC + i + 1)->line = (data + i)->line;
    (*code + IC + i + 1)->short_num = (data + i)->short_num;
  }
  free(data);
  return 1;
}
