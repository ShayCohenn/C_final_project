#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include "globals.h"
#include "utils.h"

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
  ERROR_CODE_5,
  ERROR_CODE_6,
  ERROR_CODE_7,
  ERROR_CODE_8,
  ERROR_CODE_9,
  ERROR_CODE_10,
  ERROR_CODE_11,
  ERROR_CODE_12,
  ERROR_CODE_13,
  ERROR_CODE_14
} ERROR_CODES;

extern const Error errors[];

void report_external_error(int error_code, location file);

void report_internal_error(int error_code);

#endif
