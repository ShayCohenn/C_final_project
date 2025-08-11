#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/pass1.h"
#include "../headers/utils.h"
#include "../headers/lexer.h"
#include "../headers/globals.h"
#include "../headers/errors.h"
#include "../headers/code_handler.h"

int handle_memory_alloc(symbol_table **externs, symbol_table **entries, conv_code **code, conv_code **data) {
  int err = 0;
  
  *externs = handle_malloc(sizeof(symbol_map));
  if(*externs == NULL) return 1;
  
  *entries = handle_malloc(sizeof(symbol_map));
  if(*entries == NULL) return 1;
  
  *code = handle_malloc(sizeof(conv_code));
  if(*code == NULL) return 1;
  
  *data = handle_malloc(sizeof(conv_code));
  if(*data == NULL) return 1;
  
  return 0;
}

int pass1_exe(char *file_name) {
  int IC, DC, err_code, err_found, label_table_line, externs_count, entries_count, inst_created = 1;
  
  char *str[MAX_LINE_SIZE];
  char *str_cpy[MAX_LINE_SIZE];
  symbol_table *externs, *entries;
  conv_code *code, *data;
  command_parts *command;
  inst_parts *inst;
  location am_file;
  
  err_code = 0;
  err_found = 0;
  
  label_map_line = 0;
  externs_count = 0;
  entries_count = 0;
  label_map = NULL;
  
  FILE *file;
  symbol_address *label_table;
  
  if(!valid_line_len(file_name)) err_found = 1;
  
  file = fopen(file_name, "r");
  am_file.file_name = file_name;
  am_file.line = 0;
  
  DC = 0;
  IC = -1;
  
  err_found = handle_memory_alloc(&externs, &entries, &code, &data);
  
  while(fgets(str, MAX_LINE_SIZE, file) != NULL && IC + DC <= MAX_IC - IC_INIT_VAL) {
    err_code = 0;
    (am_file.line)++;
    if(strcmp(str, "\n") == 0) continue;
    
    if(strchr(str, '.')) {
      strcpy(str_cpy, str);
      if(strstr(str_cpy, ".entry") || strstr(str_cpy, ".extern")) {
        inst = read_extern_entry(str_cpy, &err_code);
        
        if(inst->is_extern == 0)
          insert_labels(&entries, inst, am_file, ++entries_count, &err_code);
        else if(inst->is_extern == 1)
          insert_labels(&externs, inst, am_file, ++externs_count, &err_code);
      } 
      else if(strstr(str_cpy, ".data") != NULL || strstr(str_cpy, ".string") != NULL || strstr(str_cpy, ".mat")) {
        inst = read_inst(str_cpy, &err_code);
        if(inst->label != NULL)
          insert_label_table(&label_table, ++label_table_line, inst->label, DC, am_file, 1, &err_code);
        else {
          inst_created = 0;
          err_code = ERROR_CODE_25;
        }
        
        if(err_code != 0) {
          report_external_error(err_code, am_file);
          if(inst_created) free(inst);
          err_found = 1;
          continue;
        } else {
          if(inst_created) {
            if(add_machine_code_data(&data, inst, &DC, am_file) == 0) {
              err_found = 1;
              continue;
            }
          }
        }
        if(inst_created) {
          if(inst->nums) free(inst->nums);
          free(inst);
        }
      } else {
        command = read_command(str, &err_code);
        if(err_code == 0) {
          IC++;
          if(command != NULL && command->label != NULL)
            insert_labels_table(&label_table, ++label_table_line, command->label, IC, am_file, &err_code);
        } else {
          report_external_error(err_code, am_file);
          free(command);
          err_found = 1;
          continue;
        }
        
    }
  }
}
