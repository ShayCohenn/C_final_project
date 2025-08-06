#ifndef PREPROC_H
#define PREPROC_H

int is_valid_macro(char *str, char **name, int line, char *file_name);
char *macro_to_str(FILE file, fpos_t file_pos, int *line);
int macro_exec(char file_name[]);

#endif
