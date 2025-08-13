#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "../headers/utils.h"
#include "../headers/errors.h"
#include "../headers/globals.h"

int combine_bits(unsigned short a, unsigned short b, unsigned short c) {
  return ((a | b) | c);
}

int count_char_in_str(char *str, char check) {
  int count;
  char *ptr;
  ptr = str;
  count = 0;
  while((*ptr != '\0') && (ptr = strchr(ptr, check)) != NULL) {
    count++;
    ptr++;
  }
  return count;
}

int is_whitespace(char c) {
  return (isspace(c) && c != '\n');
}

void *handle_malloc(long obj_size) {
  void *obj_ptr = malloc(obj_size);
  if(obj_ptr == NULL)
    report_internal_error(ERROR_CODE_1);
  return obj_ptr;
}
/*
char *create_file(char *file_name, char *file_suff) {
  char *c, *new_file_name;
  new_file_name = handle_malloc(MAX_LINE_SIZE * sizeof(char));
  strcpy(new_file_name, file_name);
  if((c = strrchr(new_file_name, '.')) != NULL)
    *c = '\0';
  strcat(new_file_name, file_suff);
  return new_file_name;
}*/

char *create_file(const char *file_name, const char *file_suff) {
  const char *dot = strrchr(file_name, '.');
  size_t base_len = dot ? (size_t)(dot - file_name) : strlen(file_name);
  size_t suff_len = strlen(file_suff);
  size_t total = base_len + suff_len + 1;
  
  char *new_file_name = handle_malloc(total);
  if(!new_file_name) {report_internal_error(ERROR_CODE_1); return NULL;}
  memcpy(new_file_name, file_name, base_len);
  memcpy(new_file_name + base_len, file_suff, suff_len + 1);
  return new_file_name;
}

void sudden_file_close(int args_num, ...) {
   int i;
   char *str;
   FILE *file;
   va_list args;
   va_start(args, args_num);
   for(i = 0; i < args_num; i++) {
    if(strcmp(va_arg(args, char*), "%s") == 0) {
      i++;
      str = va_arg(args, char*);
      remove(str);
      free(str);
    }
    else {
      file = va_arg(args, FILE*);
      fclose(file);
    }
  }
  va_end(args);
}

int is_int(char *str) {
  char *ptr;
  if(str != NULL) {
    strtol(str, &ptr, 10);
    if(*ptr == '\0' || *ptr == ' ') return 1;
  }
  return 0;
}

int is_empty_file(FILE *file, char *mode) {
  if(file == NULL) {
    if(strcmp(mode, "r")) 
      report_internal_error(ERROR_CODE_11);
    else if(strcmp(mode, "w"))
      report_internal_error(ERROR_CODE_7);
    return 1;
  }
  return 0;
}

int copy_file(char *original_file_name, char *new_file_name) {
  char str[MAX_LINE_SIZE];
  FILE *file, *new_file;
  
  file = fopen(original_file_name, "r");
  if(is_empty_file(file, "r")) return 0;
  
  new_file = fopen(new_file_name, "w");
  if(is_empty_file(new_file, "w")) {
    fclose(file);
    return 0;
  }
  
  while(fgets(str, MAX_LINE_SIZE, file) != NULL) {
    fprintf(new_file, "%s", str);
  }
  
  fclose(file);
  fclose(new_file);
  return 1;
}
  
