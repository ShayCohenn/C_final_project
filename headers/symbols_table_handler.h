#ifndef SYMBOLS_TABLE_HANDLER_H
#define SYMBOLS_TABLE_HANDLER_H

#include "lexer.h"
#include "globals.h"

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


#endif
