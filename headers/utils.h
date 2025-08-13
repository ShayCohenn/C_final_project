#ifndef UTILS_H
#define UTILS_H

#include "globals.h"

int is_whitespace(char c);
void *handle_malloc(long obj_size);
char *create_file(const char *file_name, const char *file_suff);
void sudden_file_close(int args_num, ...);
int is_int(char *str);
int copy_file(char *original_file_name, char *new_file_name);
int is_empty_file(FILE *file, char *mode);
int count_char_in_str(char *str, char check);
int combine_bits(unsigned short a , unsigned short b, unsigned short c);

#endif
