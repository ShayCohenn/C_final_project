#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/errors.h"
#include "../headers/globals.h"

const Error errors[] = {
  {ERROR_CODE_0, "No Error"},
  {ERROR_CODE_1, "ERROR: Dynamic memory allocation failed."},
  {ERROR_CODE_2, "ERROR: Macro undefined."},
  {ERROR_CODE_3, "ERROR: Illegal macro name."},
  {ERROR_CODE_4, "ERROR: Extra text detected after macro definition."},
  {ERROR_CODE_5, "ERROR: Failed to reposition file."},
  {ERROR_CODE_6, "ERROR: .as file name missing."},
  {ERROR_CODE_7, "ERROR: Failed to open new file for writing."},
  {ERROR_CODE_8, "ERROR: Line too long"},
  {ERROR_CODE_9, "ERROR: Extra text after endmcro."},
  {ERROR_CODE_10, "ERROR: Macro defined more then once."},
  {ERROR_CODE_11, "ERROR: Failed to open new file for reading."},
  {ERROR_CODE_12, "ERROR: Failed to copy file."},
  {ERROR_CODE_13, "ERROR: mcro expansion failed."},
  {ERROR_CODE_14, "ERROR: mcro called before being declared."},
  {ERROR_CODE_15, "ERROR: Invalid label decleration."},
  {ERROR_CODE_16, "ERROR: Extra text after label decleration."},
  {ERROR_CODE_17, "ERROR: .data syntax error - may contain illegal chars."},
  {ERROR_CODE_18, "ERROR: Input number in .data out of range."},
  {ERROR_CODE_19, "ERROR: .data syntax error - comma after last number."},
  {ERROR_CODE_20, "ERROR: .data syntax error - may contain non-numeric chars."},
  {ERROR_CODE_21, "ERROR: .string syntax error - missing '\"' char after '.string'."},
  {ERROR_CODE_22, "ERROR: .string syntax error - may contain extra text after string end."},
  {ERROR_CODE_23, "ERROR: Illegal label after \".entry\"."},
  {ERROR_CODE_24, "ERROR: Invalid comma location."},
  {ERROR_CODE_25, "ERROR: Illegal data line directive."},
  {ERROR_CODE_26, "ERROR: Syntax error - illegal comma near opcode."},
  {ERROR_CODE_27, "ERROR: Illegal opcode."},
  {ERROR_CODE_28, "ERROR: Line contains extra text."},
  {ERROR_CODE_29, "ERROR: Missing arguments"},
  {ERROR_CODE_30, "ERROR: Missing comma between 2 aeguments."},
  {ERROR_CODE_32, "ERROR: Syntax error - too many commas."},
  {ERROR_CODE_33, "ERROR: Invalid first argument."},
  {ERROR_CODE_34, "ERROR: Unknown argument related error."},
  {ERROR_CODE_35, "ERROR: Extra text after arguments."},
  {ERROR_CDOE_36, "ERROR: Invalid register name."},
  {ERROR_CODE_37, "ERROR: Invalid argument."}
};

void report_internal_error(int error_code) {
  printf("ERROR:\n ID:%d | %s\n", error_code, errors[error_code].error_msg);
}

void report_external_error(int error_code, location file) {
  printf("ERROR:\n ID:%d in %s at line:%d | %s", error_code, file.file_name, file.line_num, errors[error_code].error_msg);
}
