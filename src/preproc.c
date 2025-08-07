#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/preproc.h"
#include "../headers/errors.h"
#include "../headers/globals.h"
#include "../headers/lexer.h"
#include "../headers/utils.h"
#include "../headers/text.h"

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

int add_macro(char *file_name, node **head) {
  int line, success;
  FILE file;
  fpos_t file_pos;
  char str[MAX_LINE_SIZE];
  char *name, *content;
  
  success = 1;
  
  file = fopen(file_name, "r");
  if(file == NULL) {
    report_internal_error(ERROR_CODE_11);
    success = 0;
    return success;
  }
  line = 0;
  while(fgets(str, MAX_LINE_SIZE, file)) {
    line++;
    if(strcmp(strtok(str, " "), "mcro") == 0) {
      int macro_line = line;
      if(!is_valid_macro(str, &name, line, file_name)) {
        success = 0;
        continue;
      }
      
      fgetpos(file, &file_pos);
      content = macro_to_str(file, &file_pos, &line);
      
      if(content == NULL) {
        success = 0;
        continue;
      }
      
      fsetpos(file, &file_pos);
      add_node_to_ll(head, name, content, macro_line;
    }
  }
  fclose(file, &file_pos);
  return success;
}

char *remove_macro_decleration(char file_name[]) { 
  char *word, *new_file;
  char str[MAX_LINE_SIZE];
  char str_cpy[MAX_LINE_SIZE];
  FILE *file, *output_file;
  
  file = fopen(file_name, "r");
  if(file ==NULL) {
    report_internal_error(ERROR_CODE_11);
    return NULL;
  }
  
  new_file = create_file(file_name, ".t02");
  output_file = fopen(new_file, "w");
  if(output_file == NULL) {
    report_internal_error(ERROR_CODE_7);
    sudden_file_close(4, "file", file, "%s", new_file);
    return NULL;
  }
  while(fgets(str, MAX_LINE_SIZE, file)) {
    strcpy(str_cpy, str);
    word = strtok(str, " \n";
    if(word == NULL) {
      fprintf(output_file, "\n");
      continue;
    }
    
    if(strcmp(word, "mcro") == 0){
      while(strcmp(word, "endmcro") != 0) {
        fprintf(output_file, "\n");
        fgets(str, MAX_LINE_SIZE, file);
        word = strtok(str, " \n";
        
        while(word == NULL) {
          fprintf(output_file, "\n");
          fgets(str, MAX_LINE_SIZE, file);
          word = strtok(str, " \n");
        }
      }
      fprintf(output_file, "\n");
    } else {
      fprintf(output_file, "%s", str_cpy);
    }
  }
  fclose(file);
  fclose(output_file);
  
  return new_file;
}

char *replace_one_macro(char *str, node *mcro) {
  char *mcro_pos, *new_str;
  char str_start[MAX_LINE_SIZE];
  char str_end[MAX_LINE_SIZE];
  
  strcpy(str_start, str);
  
  mcro_pos = strstr(str_start, mcro->name);
  *mcro_pos = '\0';
  
  strcpy(str_end, mcro_pos + strlen(mcro->name));
  
  new_str = handle_malloc((strlen(str_start) + strlen(mcro->content) + strlen(str_end) + 1) * sizeof(char));
  if(new_str == NULL) return NULL;
  
  strcpy(new_str, str_start);
  strcat(new_str, mcro->content);
  strcat(new_str, str_end);
  
  return new_str;
}

char *replace_all_macros(char file_name[], node *head) {
  node *mcro;
  char *mcro_pos, *new_str, *temp_file_ad, *final_file_name;
  char *str[MAX_LINE_SIZE];
  FILE *temp_file, *final_file;
  
  temp_file_ad = create_file(file_name, ".tmp");
  final_file_name = create_file(file_name, ".am");
  
  if(!

int macro_exec(char file_name[]) {
  return 1;
}
