#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_LINE_SIZE 81
#define REG_COUNT 8

typedef struct location {
  char *file_name;
  int line_num;
} location;

#endif
