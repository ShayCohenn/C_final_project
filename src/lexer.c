#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "../headers/lexer.h"
#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/errors.h"

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
    if(strcmp(str, OPCODES[i].opcode) == 0)
      return i;
  }
  return -1;
}

int is_inst(char *str) {
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

int valid_line_len(char *file_name) {
  FILE *file;
  location am_file;
  char str[MAX_LINE_SIZE + 2];
  int valid_len;
  
  file = fopen(file_name, "r");
  am_file.file_name = file_name;
  valid_len = 1;
  
  while(fgets(str, MAX_LINE_SIZE + 2, file) != NULL) {
    if(strlen(str) > MAX_LINE_SIZE) {
      (am_file.line)++;
      
      report_external_error(ERROR_CODE_8, am_file);
      valid_len = 0;
    }
  }
  fclose(file);
  return valid_len;
}

int valid_label(char *str) {
  if(str == NULL) return 0;
  
  if(isalpha(*str) && opcode_index(str) < 0 && strlen(str) <= MAX_LABEL_SIZE && !is_inst(str)) {
    while(*(++str) != '\0' && *(str) != ' ' && (isalpha(*str) || isdigit(*str))) { ; }
  }
  
  if(*str == '\0' || *str == ' ') return 1;
  else return 0;
}

int extra_text() {
  char *token;
  token = strtok(NULL, "\n");
  
  return token != NULL;
}

int add_space_after_colon(char **str, int *err_code) {
  char *ptr;
  ptr = strchr(*str, ':');
  
  if(!ptr) return 1;
  
  if(strlen(*str) == MAX_LINE_SIZE) {
    char *temp_ptr = *str;
    *str = realloc(*str, MAX_LINE_SIZE + 1);
    if(*str == NULL) {
      *err_code = ERROR_CODE_1;
      free(temp_ptr);
      return 0;
    }
  }
  
  ptr = strchr(*str, ':');
  ptr++;
  memmove(ptr+1, ptr, strlen(ptr)+1);
  *ptr = ' ';
  
  return 1;
}

int valid_label_decl(char *str, int *err_code) {
  if(str == NULL) return 0;
  
  if(strlen(str) > MAX_LABEL_SIZE || !isalpha(*str) || reg_index(str) >= 0) {
    *err_code = ERROR_CODE_15;
    return 0;
  }
  
  if(opcode_index(str) < 0) {
    while(*(++str) != '\0' && (isalpha(*str) || isdigit(*str))) { ; }
    if(*(str) ==  ':' && *(str+1) == '\0') {
      *str = '\0';
      return 1;
    } else {
      *err_code = ERROR_CODE_15;
    }
  }
  return 0;
}

int valid_str(const char *str) {
  int i, digit, comma;
  digit = 0;
  comma = 0;
  
  for(i = 0; str[i]; i++) {
    if(isdigit(str[i]) || str[i] == '-' || str[i] == ' ') {
      if(comma) comma = 0;
      if(isdigit(str[i])) digit = 1;
    } 
    else if(str[i] == ',') {
      if(!digit || comma) return 0;
      comma = 1;
    } 
    else return 0;
  }
  if(comma) return 0;
  return 1;
}

int inc_array_size(inst_parts **inst, int len) {
  short *ptr;
  ptr = (*inst)->nums;
  (*inst)->nums = realloc((*inst)->nums, (len+1) * sizeof(short));
  if((*inst)->nums == NULL) {
    report_internal_error(ERROR_CODE_1);
    free(ptr);
    return 0;
  }
  return 1;
}

int save_nums_to_arr(char *str, char *token_cpy, inst_parts *inst, int *err_code) {
  char *token;
  int len, num;
  len = 0;
  
  if(!add_space_after_colon(&token_cpy, err_code)) return 0;
  
  token = strtok(NULL, " \n");
  if(!valid_str(token)) {
    *err_code = ERROR_CODE_17;
    return 0;
  }
  strtok(token_cpy, " \n");
  strtok(NULL, " \n");
  
  while((token = strtok(NULL, ", \n")) != NULL) {
    if(is_int(token)) {
      num = (short)(atoi(token));
      if(num > MAX_NUM || num < MIN_NUM) {
        *err_code = ERROR_CODE_18;
        return 0;
      }
      else if(inc_array_size(&inst, ++len) == 0) return 0;
      *(inst->nums + len - 1) = (short)(atoi(token));
    } else {
      if(strcmp(token, "\n") == 0) {
        *err_code = ERROR_CODE_19;
        return 0;
      }
      *err_code = ERROR_CODE_20;
      return 0;
    }
  }
  inst->len = len;
  return 1;
}

int str_to_shorts_arr(char *str, inst_parts *inst, int *err_code) {
  int flag, len;
  char *token, *after;
  
  len = inst->len = 0;
  inst->nums = NULL;
  
  token = strtok(NULL, "\n");
  if(!token) {
    *err_code = ERROR_CODE_21;
    return 0;
  }
    
  while(*token && isspace((unsigned char)*token)) token++;
  if(*token != '"') {
    *err_code = ERROR_CODE_21;
    return 0;
  }
  token++;
  if(strchr(token, '"') == NULL) {
    *err_code = ERROR_CODE_21;
    return 0;
  }
  
  flag = 0;
  printf("Starting loop\ntoken:%s\ntoken+len:%s\n", token, token+len);
  while(*(token+len) != '"') {
  printf("looping %d\n token+len:%s\n", len, token+len);
    if(inc_array_size(&inst, ++len) == 0) return 0;
    *(inst->nums + len - 1) = (short)(*(token + len - 1));
    printf("inst->nums %hn\n", inst->nums);
    flag = 1;
  }
  printf("ended loop\n%s\n", token+len+1);
  after = token + len + 1;
  while(*after && isspace((unsigned char)*after)) after++;
  printf("after: %s\n", after);
  if(*after != '\0'){
    printf("inside if: %s\n", (token+len+1));
    *err_code = ERROR_CODE_22;
    return 0;
  }
  if(inc_array_size(&inst, len+1) == 0) return 0;
  
  *(inst->nums + len) = 0;
  
  inst->len = len + 1;
  printf("inst->len = %d, len = %d, inst->nums = %hn\n", inst->len, len, inst->nums);
  return 1;
}

int comma_after_str(char *str, int *err_code) {
  if(strchr(str,'.')) {
    *err_code = ERROR_CODE_24;
    return 1;
  }
  *err_code = ERROR_CODE_25;
  return 0;
}

inst_parts *read_inst(char *str, int *err_code) {
  inst_parts *inst;
  char *token;
  char token_cpy[MAX_LINE_SIZE];
  strcpy(token_cpy, str);
  
  if(strstr(str, ".") == NULL) return 0;
  
  if(!add_space_after_colon(&str, err_code)) return NULL;
  
  token = strtok(str, " \n");
  
  inst = handle_malloc(sizeof(inst_parts));
  if(inst == NULL) {
    *err_code = ERROR_CODE_1;
    return NULL;
  }
  
  inst->label = NULL;
  inst->nums = NULL;
  
  if(valid_label_decl(token, err_code)) {
    inst->label = token;
    token = strtok(NULL, " \n");
  } else if(strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0 || strcmp(token, ".mat") == 0 || strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0) {
    inst->label = NULL;
  }
  
  if(strcmp(token,".data") == 0)
    save_nums_to_arr(str, token_cpy, inst, err_code);
  else if(strcmp(token, ".string") == 0)
    str_to_shorts_arr(str,inst,err_code);
  else if(strcmp(token, ".mat") == 0) {
    return 0;
  }
  else if(strcmp(token, ".entry") == 0) {
    token = strtok(NULL, " \n");
    if(valid_label(token)) {
      inst->label = NULL;
      inst->len = -1;
      inst->arg_label = token;
      inst->nums = 0;
      inst->is_extern = 0;
    } else {
      *err_code = ERROR_CODE_23;
      free(inst);
      return 0;
    }
  } else if(strcmp(token, ".extern") == 0) {
    token = strtok(NULL, " \n");
    if(valid_label(token)) {
      inst->label = NULL;
      inst->len = -1;
      inst->arg_label = token;
      inst->nums = 0;
      inst->is_extern = 1;
    }
  } else {
    comma_after_str(token, err_code);
  }
  
  return inst;
}

inst_parts *read_extern_entry(char *str, int *err_code) {
  inst_parts *inst;
  char *ptr, *token;
  
  ptr = strchr(str, '.');
  token = strtok(ptr,  " ");
  
  inst = handle_malloc(sizeof(inst_parts));
  
  if(inst == NULL) {
    *err_code = ERROR_CODE_1;
    return NULL;
  }
  
  inst->label = NULL;
  inst->nums = NULL;
  inst->is_extern = 0;
  inst->len = 0;
  
  if(strcmp(token, ".extern") == 0) inst->is_extern = 1;
  
  token = strtok(NULL, " \n");
  if(valid_label(token)) inst->arg_label = token;
  else *err_code = ERROR_CODE_15;
  
  if(extra_text()) *err_code = ERROR_CODE_16;
  
  return inst;
}  

int opcode_error_check(char *str) {
  char *c;
  if((c = strchr(str, ',')) != NULL) return ERROR_CODE_26;
  return ERROR_CODE_27;
}

int label_reg_or_num(char *str) {
  return ((reg_index(str) >= 0) || (valid_label(str)) || (is_int(str)));
}

int check_first_arg(char *str, char *ptr) {
  int first_arg_len = (int)(ptr-str);
  char first_arg[MAX_LINE_SIZE];
  strncpy(first_arg, str, first_arg_len);
  first_arg[first_arg_len] = '\0';
  return ((reg_index(first_arg) >= 0) || (valid_label(first_arg)) || (is_int(first_arg)));
}

int valid_mat(char *str) {
  return 0;
}

int is_valid_arg(char *str, command_parts *command, int *err_code) {
  char *ptr, *str1, *str2;
  
  if(str == NULL && OPCODES[command->opcode].arg_num != 0) {
    *err_code = ERROR_CODE_29;
    return 0;
  }
  if(OPCODES[command->opcode].arg_num == 0) {
    if(extra_text()) {
      *err_code = ERROR_CODE_28;
      return 0;
    } else {
      command->source = NULL;
      command->dest = NULL;
      return 1;
    }
  }
  
  if(OPCODES[command->opcode].arg_num == 2) {
    if(strstr(str, ",") == NULL) {
      *err_code = ERROR_CODE_30;
      return 0;
    } else if(count_char_in_str(str, ',') > 1) {
        *err_code = ERROR_CODE_31;
        return 0;
    } else {
      str1 = strtok(str,",");
      if((ptr = strchr(str1, ' '))) {
        if(check_first_arg(str1, ptr)) {
          *err_code = ERROR_CODE_33;
          return 0;
        }
        *err_code = ERROR_CODE_34;
        return 0;
      }
      str2 = strtok(NULL, " \n");
      if(extra_text()) {
        *err_code = ERROR_CODE_35;
        return 0;
      }
    }
  }
  else if(OPCODES[command->opcode].arg_num == 1) {
    if(strchr(str, ' ')) {
      *err_code = ERROR_CODE_35;
      return 0;
    }
  }
  
  switch(command->opcode) {
    case 1: {
      if(label_reg_or_num(str1) && label_reg_or_num(str2)) {
        command->source = str1;
        command->dest = str2;
      } else {
        if(str2 == NULL) *err_code = ERROR_CODE_29;
        else if(reg_index(str1) == -1 || reg_index(str2) == -1) *err_code = ERROR_CODE_36;
        else *err_code = ERROR_CODE_37;
        return 0;
      }
      break;
    }
  }
  return 1;
}
      

command_parts *read_command(char *str, int *err_code) {
  char *token;
  int flag;
  command_parts *command;
  
  flag = 0;
  command = handle_malloc(sizeof(command_parts));
  
  if(command == NULL) return command;
  
  if(!add_space_after_colon(&str, err_code)) return command;
  
  token = strtok(str, " \n");
  if(valid_label_decl(token, err_code)) {
    flag = 1;
    command->label = token;
    token = strtok(NULL, " \n");
    if((command->opcode = opcode_index(token)) != -1) { ; }
    else {
      *err_code = opcode_error_check(token);
      command->opcode = -1;
      return command;
    }
    
    if(OPCODES[command->opcode].arg_num == 0) {
      if(extra_text()) *err_code = ERROR_CODE_28;
      else {
      command->source = NULL;
      command->dest = NULL;
      }
    } else {
      if(is_valid_arg(strtok(NULL, "\n"), command, err_code) == 0) return command;
    }
  }
  else {
    if(*err_code) return command;
  }
  if(!flag) {
    if((command->opcode = opcode_index(token)) != -1) {
      command->label = NULL;
      is_valid_arg(strtok(NULL, "\n"), command, err_code);
      return command;
    } else {
      *err_code = opcode_error_check(token);
      command->opcode = -1;
      return command;
    }
  } else {
    return command;
  }
}
