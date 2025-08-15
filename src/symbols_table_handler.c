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
  if(table == NULL) {
    *err_code = ERROR_CODE_1;
    free(ptr);
    return 0;
  }
  return 1;
}

int insert_label_table(symbol_address **label_table, int lines, char *label, int count, location am_file, int is_data_line, int *err_code) {
  return 0;
}

int check_table_labels(symbol_address *label_table, int lines, char *file_name) {
  int i, j;
  
  for(i = 0; i < lines; i++) {
    for(j = 0; j < lines; j++) {
      if(strcmp((label_table + i)->name, (label_table + j)->name) == 0) {
        location am_file;
        am_file.file_name = file_name;
        am_file.line = (label_table + j)->line;
        report_external_error(ERROR_CODE_39, am_file);
        return 0;
      }
    }
  }
  return 1;
}

int extern_defined(symbol_table *externs, int externs_count, symbol_address *label_table, int line, char *file_name) {
  int i, j, extern_def, flag;
  location am_file;
  extern_def = 0;
  
  for(i = 0; i < externs_count; i++) {
    flag = 0;
    for(j = 0; j < line && flag == 0; j++) {
      if(strcmp((externs + i)->name, (label_table + j)->name) == 0) {
        extern_def = 1;
        flag = 0;
        am_file.file_name = file_name;
        am_file.line = (label_table + j)->line;
        report_external_error(ERROR_CODE_40, am_file);
      }
    }
  }
  return extern_def;
}

void update_data_address(symbol_address *label_table, int table_lines, int IC) {
  int i;
  for(i = 0; i < table_lines; i++) {
    if((label_table + i)->is_data_line) (label_table + i)->address += IC + 1;
  }
}

void reset_label_address(symbol_address *label_table, int table_lines) {
  int i;
  for(i = 0; i < table_lines; i++) {
    (label_table + i)->address += IC_INIT_VAL;
  }
}

void replace_externs_value(conv_code *code, symbol_table *externs, int externs_count, int count, char *file_name) {
  int i, j, found;
  for(i = 0; i <= count; i++) {
    found = 0;
    if((code + i)->label != NULL) {
      for(j = 0; j < externs_count && found == 0; j++) {
        if(strcmp((code + i)->label, (externs + j)->name) == 0) {
          (code + i)->short_num -= 1;
          found = 1;
        }
      }
    }
  }
}

int set_label_address(conv_code *code, symbol_address *label_table, int line, int IC, char *file_name) {
  int i, j, flag, err_found;
  err_found = 1;
  for(i = 0; i <= IC; i++) {
    flag = 0;
    if((code + i)->label != NULL && (code + i)->short_num != 1) {
      for(j = 0; j < line && flag == 0; j++) {
        if(strcmp((code + i)->label, (label_table + j)->name) == 0) {
          (code + i)->short_num |= ((label_table + j)->address) << ARE_BITS;
          flag = 1;
        }
      }
      
      if(!flag) {
        location am_file;
        am_file.file_name = file_name;
        am_file.line = (code + i)->line;
        report_external_error(ERROR_CODE_15, am_file);
        err_found = 0;
      }
    }
  }
  return err_found;
}
