#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kycommandline.h"
#include "processor.h"

int main(){
  char* commandline;
  char** args;
  int numargs;

  while(1){
    printf("TCKY$: ");
    
    commandline = read_line();
    numargs = count_args(commandline);
    args = separate_line(commandline);

    process(args, numargs);
  
  }
  
  return 1;
}
