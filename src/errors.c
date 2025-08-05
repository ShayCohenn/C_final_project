#include <stdio.h>
#include "../headers/errors.h"

Error errors[] {
  {ERROR_CODE_0, "No Error"},
  {ERROR_CODE_1, "Dynamic memory allocation failed."}
};

void report_internal_error(int error_code) {
  printf("ERROR:\n ID:%d | %s\n", error_code, errors[error_code].error_msg);
}
