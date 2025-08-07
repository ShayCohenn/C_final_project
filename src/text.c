#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../headers/text.h"
#include "../headers/preproc.h"
#include "../headers/errors.h"
#include "../headers/globals.h"
#include "../headers/utils.h"

char *file_to_string(FILE *file, fpos_t file_pos, int len) {
  int i;
  char *str;
  if(fsetpos(file, file_pos) != 0) {
    report_internal_error(ERROR_CODE_5);
    return NULL;
  }
  str = handle_malloc((len + 1) * sizeof(char));
  for(i = 0; i < len; i++) {
    *(str + i) = getc(file);
  }
  *(str + i) = '\0';
  fgetpos(file, file_pos);
  return str;
}

void whitespace_remove_comma(char *str) {
  char *ptr = str;
  if(*ptr == ',') return;
  
  while((ptr = strchr(ptr, ',')) != NULL) {
  if(*(ptr-1) == ' ') {
    memmove(ptr - 1, ptr, strlen(ptr) + 1);
    if(*(ptr) == ' ') 
      memmove(ptr, ptr+1, strlen(ptr+1) + 1);
  }
  else if(*(ptr + 1) == ' ') {
    memmove(ptr + 1, ptr + 2, strlen(ptr + 2) + 1);
    ptr++;
  } else {
      ptr++;
    }
  }
}

void whitespace_remove_str(char str[]) {
  int i,j;
  char str_temp[MAX_LINE_SIZE];
  j = i = 0;
  while(is_whitespace(*(str+i))) {
    i++;
  }
  while(*(str+i) != '\0') {
    while(!is_whitespace(*(str+i)) && *(str+i) != '\0') {
      *(str_temp + j) = *(str + i);
      i++;
      j++;
    }
    if(*(str+i) == '\0') break;
    
    while(is_whitespace(*(str+i))) {
      i++;
    }
    if(*(str+i) != '\n' && *(str+i) != '\0') {
      *(str_temp+j) = ' ';
      j++;
    }
  }
  *(str_temp+j) = *(str+i);
  *(str_temp+j+1) = '\0';
  whitespace_remove_comma(str_temp);
  strcpy(str, str_temp);
}

char *whitespace_remove_file(char file_name[]) {
  char *new_file_name;
  char str[MAX_LINE_SIZE];
  int line;
  FILE *file, *file_temp;
  file = fopen(file_name, "r");
  if(file == NULL) {
    report_internal_error(ERROR_CODE_6);
    return NULL;
  }
  
  new_file_name = add_new_file(file_name, ".t01");
  if(new_file_name == NULL) {
    sudden_file_close(2, "file", file);
    return NULL;
  }
  
  file_temp = fopen(new_file_name, "w");
  if(file_temp == NULL) {
    sudden_file_close(4, "file", file, "%s", new_file_name);
    report_internal_error(ERROR_CODE_7);
    return NULL;
  }
  
  line = 0;
  while(fgets(str, MAX_LINE_SIZE+2, file) != NULL) { 
    line++;
    if(strlen(str) > MAX_LINE_SIZE) {
      location as_file;
      as_file.file_name = file_name;
      as_file.line_num = line;
      report_external_error(ERROR_CODE_8, as_file);
      fclose(file);
      fclose(file_temp);
      return NULL;
    }
    else if (*str == ";") {
      *str = '\n';
      *(str+1) = '\0';
    } else {
      whitespace_remove_str(str);
    }
    fprintf(file_temp, "%s", str);
  }
  fclose(file);
  fclose(file_temp);
  
  return new_file_name;
}  
