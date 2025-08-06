#ifndef TEXT_H
#define TEXT_H

void whitespace_remove_comma(char *str);
void whitespace_remove_str(char str[]);
char *file_to_string(FILE *file, fpos_t *file_pos, int len);
char *whitespace_remove_file(char file_name[]);
