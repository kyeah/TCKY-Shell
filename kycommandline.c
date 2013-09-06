#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "kycommandline.h"

char* read_line(){
//reads the input from the commandline and returns a string that is the commandline string
  char* dest = (char*)malloc(sizeof(char) * 256);
  fgets(dest, 256, stdin);
  return dest;
}

char** separate_line(char* line){
//Separates each argument in line into an array of strings
  char** ans = (char**)malloc(sizeof(char*) * 256);

  int i = 0;
  while( ans[i] = strsep(&line, " ") )
    i++;
  
  //Remove whitespace at the end of last argument
  if(i)
    ans[i-1][strlen(ans[i-1]) - 1] = 0;

  return ans;
}

int count_args(char* line){
//Returns the number of args in line by counting the number of spaces
  int numspaces = 0;
  int i;
  for(i=0;line[i];i++)
    if(line[i] == ' ')
      numspaces++;

  return numspaces + 1;
}
