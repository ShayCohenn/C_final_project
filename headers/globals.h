#ifndef GLOBALS_H
#define GLOBALS_H

#define MEMORY_SIZE 255
#define BITS 10
#define MAX_LINE_SIZE 81
#define MAX_LABEL_SIZE 30
#define MAX_IC 255
#define IC_INIT_VAL 100
#define REG_COUNT 8
#define MAX_NUM ((1 << (BITS-1)) - 1)
#define MIN_NUM (-(1 << (BITS-1)))

#define DIRECT_ADDRESSING 0
#define LABEL_ADDRESSING 1
#define MAT_ADDRESSING 2
#define REG_ADDRESSING 3

#define SRC_BITS_SHIFT 4
#define DEST_BITS_SHIFT 2
#define OPCODE_BITS_SHIFT 6

#define REG_SRC_BITS_SHIFT 6
#define REG_DEST_BITS_SHIFT 2

#define ARE_BITS 2


typedef struct location {
  char *file_name;
  int line;
} location;

#endif
