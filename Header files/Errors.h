#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include "globals.h"

typedef struct Error {
  int error_id;
  char *error_msg;
} Error;

typedef enum ERROR_CODES {
  ERROR_CODE_0 = 0,
  ERROR_CODE_1,
  ERROR_CODE_2,
  ERROR_CODE_3,
  ERROR_CODE_4,
  ERROR_CODE_5
} ERROR_CODES;

void print_external_error(int error_code, location file);

void print_internal_error(int error_code);

#endif
