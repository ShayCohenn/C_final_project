#ifndef PREPROC_H
#define PREPROC_H

#include <stdio.h>
#include "linked_list_handler.h"

int is_valid_macro(char *str, char **macro_name, int line, char *file_name);
char *macro_to_str(FILE *file, fpos_t *file_pos, int *line);
int add_macro(char *file_name, node **head);
char *replace_all_macros(char file_name[], node *head);
char *remove_macro_decleration(char file_name[]);
char *replace_one_macro(char *str, node *mcro);
int check_macro_decl_order(char file_name[], node *head);
int macro_exec(char file_name[]);

#endif
