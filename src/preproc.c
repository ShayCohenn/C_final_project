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


