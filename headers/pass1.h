#ifndef PASS1_H
#define PASS1_H

#include "code_handler.h"
#include "symbols_table_handler.h"

int handle_memory_alloc(symbol_table **externs, symbol_table **entries, conv_code **code, conv_code **data);
int pass1_exe(char *file_name);

#endif
