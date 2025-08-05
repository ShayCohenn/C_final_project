#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/utils.h"
#include "../headers/errors.h"
#include "../headers/globals.h"

void *handle_malloc(long obj_size) {
  void *obj_ptr = malloc(obj_size);
  if(obj_ptr == NULL)
    report_internal_error(ERROR_CODE_1);
  return obj_ptr;
}

char *add_new_file(char *file_name, char *file_suff) {
  char *c, *new_file_name;
  new_file_name = handle_malloc(MAX_LINE_SIZE * sizeof(char));
  strcpy(new_file_name, file_name);
  if((c = strchr(new_file_name, '.')) != NULL)
    *c = '\0';
  strcat(new_file_name, file_suff);
  return new_file_name;
}
