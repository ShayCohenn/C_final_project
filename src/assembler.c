#include <stdio.h>
#include <stdlib.h>

#include "../headers/utils.h"
#include "../headers/preproc.h"

int main(int args_num, char *args[]) {
  char *as_file, *am_file;
  while(--args_num > 0) {
    printf("Starting the pre-proccess\n");
    as_file = create_file(args[args_num], ".as");
    
    if(!macro_exec(as_file)) continue;
    
    printf("First pass:\n");
  }
  return 0;
}
