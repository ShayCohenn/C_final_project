#ifndef PASS2_H
#define PASS2_H

#include "symbols_table_handler.h"
#include "code_handler.h"

int pass2_exe(char *file_name, symbol_address *lable_table, int IC, int DC, int label_table_line, int externs_count, int entries_count, conv_code *code, conv_code *data, symbol_table *externs, symbol_table *entries, int err_found); 

#endif
