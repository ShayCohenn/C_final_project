#ifndef LEXER_H
#define LEXER_H

typedef struct opcode {
  char *opcode;
  int arg_num;
} opcode;

#define OPCODES_COUNT 16
extern opcode OPCODE[];

typedef struct command_parts {
  char *label;
  int opcode;
  char *source;
  char *dest;
} command_parts;

typedef struct inst_parts {
  char *label;
  int *nums;
  int len;
  char *arg_label;
  int is_extern;
} inst_parts;

int opcode_index(char *str);
int is_inst(char *str);
int reg_index(char *str);
int valid_line_len(char *file_name);
inst_parts *read_extern_entry(char *str, int *err_code);
int valid_label(char *str);
int extra_text();
inst_parts *read_inst(char *str, int *err_code);
int add_space_after_colon(char **str, int *err_code);
int valid_label_decl(char *str, int *err_code);
int save_nums_to_arr(char *str, char *token_cpy, inst_parts *inst, int *err_code);
int str_to_shorts_arr(char *str, inst_parts *inst, int *err_code);
int valid_str(const char *str);
int inc_array_size(inst_parts **inst, int len);
int comma_after_str(char *str, int *err_code);
command_parts *read_command(char *str, int *err_code);
int opcode_error_check(char *str);
int is_valid_arg(char *str, command_parts *command, int *err_code);
int check_first_arg(char *str, char *ptr);
int label_reg_or_num(char *str);
int valid_mat(char *str);



#define INSTRUCTION_COUNT 5

#define REG_COUNT 8

#endif
