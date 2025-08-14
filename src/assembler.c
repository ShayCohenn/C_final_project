#include <stdio.h>
#include <stdlib.h>

#include "../headers/utils.h"
#include "../headers/preproc.h"
#include "../headers/pass1.h"

int main(int args_num, char *args[]) {
  char *as_file, *am_file;
  while(--args_num > 0) {
    printf("Starting the pre-proccess\n");
    as_file = create_file(args[args_num], ".as");
    
    if(!macro_exec(as_file)) continue;
    printf("First pass:\n");
    
    am_file = create_file(args[args_num], ".am");
    printf("am file created\n");
    if(pass1_exe(am_file)) {
      continue;
    }
    printf("finished first pass \n");
    
    free(as_file);
    free(am_file);
  }
  
  return 0;
}
