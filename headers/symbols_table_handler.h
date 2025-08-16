#ifndef SYMBOLS_TABLE_HANDLER_H
#define SYMBOLS_TABLE_HANDLER_H

#include "lexer.h"
#include "globals.h"
#include "code_handler.h"

typedef struct symbol_table {
  char *name;
  int line;
} symbol_table;

typedef struct symbol_address {
  int address;
  char *name;
  int line;
  int is_data_line;
} symbol_address;

int insert_labels(symbol_table **table, inst_parts *inst, location am_file, int count, int *err_code);
int insert_label_table(symbol_address **label_table, int lines, char *label, int count, location am_file, int is_data_line, int *err_code);
int check_table_labels(symbol_address *label_table, int lines, char *file_name);
int extern_defined(symbol_table *externs, int externs_count, symbol_address *label_table, int line, char *file_name);
void update_data_address(symbol_address *label_table, int table_lines, int IC);
void reset_label_address(symbol_address *label_table, int table_lines);
void replace_externs_value(conv_code *code, symbol_table *externs, int externs_count, int count, char *file_name);
int set_label_address(conv_code *code, symbol_address *label_table, int line, int IC, char *file_name);
int extern_file(conv_code *code, int count, symbol_table *externs, int externs_count, char *file_name);
int entries_file(symbol_address *label_table, int line, symbol_table *entries, int entries_count, char *file_name);
void free_symbol_table(symbol_address *label_table, int line);
void free_other_tables(symbol_table *table, int count);

#endif
