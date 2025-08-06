#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/preproc.h"
#include "../headers/errors.h"
#include "../headers/globals.h"
#include "../headers/lexer.h"
#include "../headers/utils.h"

int is_valid_macro(char *str, char **name, int line, char *file_name) {
  char *temp_name, *extra;
  temp_name = strtok(NULL, " \n");
  if (temp_name == NULL) {
    report_internal_error(ERROR_CODE_2);
    return 0;
  }
  if(is_instrument(temp_name) || opcode_index(temp_name) >= 0 || reg_index(temp_name) >= 0) {
    location as_file;
    as_file.file_name = file_name;
    as_file.line_num = line;
    report_external_error(ERROR_CODE_3, as_file);
    return 0;
  }
  extra = strtok(NULL, "\n");
  if(extra != NULL) {
    report_internal_error(ERROR_CODE_4);
    return 0;
  }
  
  *macro_name = handle_malloc((strlen(temp_name) + 1) * sizeof(char));
  strcpy(*name, temp_name);
  
  return 1;
}

char *macro_to_str(FILE *file, fpos_t *file_pos, int *line) {
  int macro_len;
  char str[MAX_LINE_SIZE];
  if(fsetpos(file, file_pos) != 0) {
    report_internal_error(ERROR_CODE_5);
    return NULL;
  }
  macro_len = 0;
  str[0] = '\0';
  
  while(fgets(str, MAX_LINE_SIZE, file) && (strcmp(str, "endmcro\n")) != 0) {
    if((strstr(str, "endmcro") != NULL) && strlen(str) != strlen("endmcro")) {
      report_internal_error(ERROR_CODE_9);
      return NULL;
    }
    (*line)++;
    if(strcmp(str, "endmcro\n") != 0) 
      macro_len += strlen(str);
  }
  
  mcro = copy_text(file, file_pos, macro_len);
  return mcro;
}

int macro_exec(char file_name[]) {
  return 1;
}
