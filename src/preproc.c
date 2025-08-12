#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/preproc.h"
#include "../headers/errors.h"
#include "../headers/globals.h"
#include "../headers/lexer.h"
#include "../headers/utils.h"
#include "../headers/text.h"

int is_valid_macro(char *str, char **macro_name, int line, char *file_name) {
  char *temp_name, *extra;
  temp_name = strtok(NULL, " \n");
  if (temp_name == NULL) {
    report_internal_error(ERROR_CODE_2);
    return 0;
  }
  if(is_inst(temp_name) || opcode_index(temp_name) >= 0 || reg_index(temp_name) >= 0) {
    location as_file;
    as_file.file_name = file_name;
    as_file.line = line;
    report_external_error(ERROR_CODE_3, as_file);
    return 0;
  }
  extra = strtok(NULL, "\n");
  if(extra != NULL) {
    report_internal_error(ERROR_CODE_4);
    return 0;
  }
  
  *macro_name = handle_malloc((strlen(temp_name) + 1) * sizeof(char));
  strcpy(*macro_name, temp_name);
  
  return 1;
}

char *macro_to_str(FILE *file, fpos_t *file_pos, int *line) {
  int macro_len;
  char str[MAX_LINE_SIZE];
  char *mcro;
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
  FILE *file;
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
      add_node_to_ll(head, name, content, macro_line);
    }
  }
  fclose(file);
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
    word = strtok(str, " \n");
    if(word == NULL) {
      fprintf(output_file, "\n");
      continue;
    }
    
    if(strcmp(word, "mcro") == 0){
      while(strcmp(word, "endmcro") != 0) {
        fprintf(output_file, "\n");
        fgets(str, MAX_LINE_SIZE, file);
        word = strtok(str, " \n");
        
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
  char *mcro_pos, *new_str, *temp_file_name, *final_file_name;
  char str[MAX_LINE_SIZE];
  FILE *temp_file, *final_file;
  
  temp_file_name = create_file(file_name, ".tmp");
  final_file_name = create_file(file_name, ".am");
  
  if(!copy_file(file_name, temp_file_name) || !copy_file(file_name, final_file_name)) {
    report_internal_error(ERROR_CODE_12);
    report_internal_error(ERROR_CODE_13);
    sudden_file_close(4, "%s", temp_file_name, "%s", final_file_name);
    return NULL;
  }
  
  mcro = head;
  while(mcro != NULL) {
    temp_file = fopen(temp_file_name, "r");
    if(is_empty_file(temp_file, "r")) {
      sudden_file_close(4, "%s", temp_file_name, "%s", final_file_name);
      return NULL;
    }
    
    final_file = fopen(final_file_name, "w");
    if(is_empty_file(final_file, "w") == 0) {
      sudden_file_close(6, "file", temp_file, "%s", temp_file_name, "%s", final_file_name);
      return NULL;
    }
    
    while(fgets(str, MAX_LINE_SIZE, temp_file)) {
      mcro_pos = strstr(str, mcro->name);
      if(mcro_pos != NULL) {
        *(str + strlen(str) - 1) = '\0';
        new_str = replace_one_macro(str, mcro);
        if(new_str == NULL) {
          sudden_file_close(8, "file", final_file, "file", temp_file, "%s", temp_file_name, "%s", final_file_name);
          return NULL;
        }
        
        fprintf(final_file, "%s", new_str);
        free(new_str);
      } else {
        fprintf(final_file, "%s", str);
      }
    }
    
    fclose(temp_file);
    fclose(final_file);
    
    mcro = mcro->next;
    if(mcro == NULL) break;
    
    remove(temp_file_name);
    rename(final_file_name, temp_file_name);
  }
  remove(temp_file_name);
  free(temp_file_name);
    
  return final_file_name;
}

int check_macro_decl_order(char file_name[], node *head) {
  FILE *file;
  int line, valid;
  node *mcro;
  char str[MAX_LINE_SIZE];
  
  file = fopen(file_name, "r");
  valid = 0;
  line = 0;
  while(fgets(str, MAX_LINE_SIZE, file) != NULL) {
    line++;
    if(strstr(str, "mcro") != NULL) continue;
    mcro = head;
    while((mcro != NULL) && (mcro->line < line)) {
      mcro = mcro->next;
    }
    
    if(mcro == NULL) continue;
    
    while (mcro != NULL) {
      if(strstr(str, mcro->name) != NULL) {
        report_internal_error(ERROR_CODE_14);
        valid = 0;
      }
      mcro = mcro->next;
    }
  }
  fclose(file);
  return valid;
}
    

int macro_exec(char file_name[]) {
  node *head;
  char *new_file1, *new_file2, *temp_file_name1, *temp_file_name2, *final_file;
  
  new_file1 = whitespace_remove_file(file_name);
  if(new_file1 == NULL) return 0;
  
  head = NULL;
  
  if(!add_macro(new_file1, &head)) {
    delete_ll(head);
    sudden_file_close(2, "%s", new_file1);
    return 0;
  }
  
  new_file2 = remove_macro_decleration(new_file1);
  if(new_file2 == NULL) {
    delete_ll(head);
    sudden_file_close(2, "%s", new_file1);
    report_internal_error(ERROR_CODE_13);
    return 0;
  }
  
  free(new_file1);
  
  final_file = replace_all_macros(new_file2, head);
  if(final_file == NULL) {
    delete_ll(head);
    sudden_file_close(2, "%s", new_file2);
    report_internal_error(ERROR_CODE_13);
    return 0;
  }
  
  temp_file_name1 = create_file(file_name, ".t01");
  temp_file_name2 = create_file(file_name, ".t02");
  
  remove(temp_file_name1);
  remove(temp_file_name2);
  
  free(temp_file_name1);
  free(temp_file_name2);
  
  free(new_file2);
  free(final_file);
  delete_ll(head);
   
  return 1;
}
