#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../headers/pass2.h"
#include "../headers/symbols_table_handler.h"
#include "../headers/code_handler.h"
#include "../headers/errors.h"
#include "../headers/utils.h"

int pass2_exe(char *file_name, symbol_address *label_table, int IC, int DC, int line, int externs_count, int entries_count, conv_code *code, conv_code *data, symbol_table *externs, symbol_table *entries, int err_found) {

  if(IC > MAX_IC) { report_internal_error(ERROR_CODE_38); err_found = 1; }
  
  if(check_table_labels(label_table, line, file_name) == 0) err_found = 1;
  
  if(extern_defined(externs, externs_count, label_table, line, file_name) == 1) err_found = 1;
  
  update_data_address(label_table, line, IC);
  reset_label_address(label_table, line);
  
  if(join_data_code(&code, data, IC, DC) == 0) err_found = 1;
  
  replace_externs_value(code, externs, externs_count, IC+DC, file_name);
  
  if(set_label_address(code, label_table, line, IC, file_name) == 0) err_found = 1;
  
  if(err_found = 0) {
    code_to_base4(code, file_name, IC, DC);
    extern_file(code, externs, externs_count, file_name);
    entries_file(label_table, line, entries, entries_count, file_name);
  }
  free_memory(code, label_table, entries, externs, IC + DC, line, entries_count, externs_count);
  return !err_found;
}
