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


typedef struct location {
  char *file_name;
  int line;
} location;

#endif
