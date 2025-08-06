#ifndef UTILS_H
#define UTILS_H

int is_whitespace(char c);
void *handle_malloc(long obj_size);
char *create_new_file(char *file_name, char *file_suff);
void abrupt_close(int args_num, ...);


#endif
