#include <stdio.h>
#include "../headers/errors.h"
#include "../headers/globals.h"

Error errors[] {
  {ERROR_CODE_0, "No Error"},
  {ERROR_CODE_1, "ERROR: Dynamic memory allocation failed."},
  {ERROR_CODE_2, "ERROR: Macro undefined."},
  {ERROR_CODE_3, "ERROR: Illegal macro name."},
  {ERROR_CODE_4, "ERROR: Extra text detected after macro definition."},
  {ERROR_CODE_5, "ERROR: Failed to reposition file."},
  {ERROR_CODE_6, "ERROR: .as file name missing."},
  {ERROR_CODE_7, "ERROR: Failed to open new file for writing."},
  {ERROR_CODE_8, "ERROR: Line too long"},
  {ERROR_CODE_9, "ERROR: Extra text after endmcro."}
};

void report_internal_error(int error_code) {
  printf("ERROR:\n ID:%d | %s\n", error_code, errors[error_code].error_msg);
}

void report_external_error(int error_code, location file) {
  printf("ERROR:\n ID:%d in %s at line:%d | %s", error_code, file.file_name, file.line_num, errors[error_code].error_msg);
}
