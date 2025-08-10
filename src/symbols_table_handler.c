#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/symbols_table_handler.h"
#include "../headers/errors.h"
#include "../headers/utils.h"
#include "../headers/globals.h"

int insert_labels(symbol_table **table, inst_parts *inst, location am_file, int count, int *err_code) {
  symbol_table *ptr;
  int label_len;
  
  if(inst->arg_label == NULL) return 0;
  
  ptr = *table;
  (*table + count - 1)->line = am_file.line;
  
  label_len = strlen(inst->arg_label) + 1;
  
  (*table + count - 1)->name = handle_malloc(label_len * sizeof(char));
  if((*table + count - 1)->name == NULL) {
    *err_code = ERROR_CODE_1;
    return 0;
  }
  
  strcpy((*table + count - 1)->name, inst->arg_label);
  
  *table = realloc(*table, (count + 1) * sizeof(symbol_table));
  if(table == NULL {
    *err_code = ERROR_CODE_1;
    free(ptr);
    return 0;
  }
  return 1;
}
