#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "processor.h"
#include "kycommandline.h"

char* redir[] = {">", "<", ">>", "2>", "|"};

int isRedirectSymbol(char* s){
//Returns 1 if string is a redirection symbol
  int i;
  for(i=0; i<5; i++)
    if(!strncmp(s, redir[i], 2))
      return 1;
  
  return 0;
}

int semicolonFound(char* s){
//Returns 1 if the string ends in a semicolon
  return s[ strlen(s) - 1 ] == ';';
}

void remove_semicolon(char* s){
//turns all semicolons at the end of string s into null
  while(semicolonFound(s))
    s[ strlen(s) - 1 ] = 0;
}

int isolate_command(char** args, int numargs, int args_spot, char** command){
//Isolates the next full command in <args> and inserts it into <command>
//Returns the current <args_spot>
  int cmd_spot = 0;
  
  while( args_spot < numargs &&
	 args[args_spot] &&
	 !isRedirectSymbol(args[args_spot]) )
    {
      command[cmd_spot] = args[args_spot];
      args_spot++;
      cmd_spot++;

      if(semicolonFound(command[cmd_spot-1])){
	remove_semicolon(command[cmd_spot-1]);
	break;
      }   
    }

  return args_spot;
}

void process(char** args, int numargs){
//Evaluates the user input given by <args> based on the number of args <numargs>

  char** command = (char**)malloc(sizeof(char*) * 256);
  int args_spot = 0;
//args_spot keeps track of the current arg being read in the full user input array

  while(args_spot < numargs){
    args_spot = isolate_command(args, numargs, args_spot, command);

    //Will execute a single command if there is a semicolon or the end of the input string has been reached
    if( args_spot >= numargs || !isRedirectSymbol(args[args_spot]) )
      execute(command);

    //Otherwise, there is redirection/piping involved.
    else{
      int stdout_save = dup(STDOUT_FILENO);
      int stdin_save = dup(STDIN_FILENO);
      int stderror_save = dup(STDERR_FILENO);

      int piped = 0;   //Holds 1 if piping occurs                                       

      char* redir_option;
      char** command2 = (char**)malloc(sizeof(char*) * 256);
      char** pipedcmd = (char**)malloc(sizeof(char*) * 256);

      //While not at semicolon or end of input
      while( args[args_spot] && isRedirectSymbol(args[args_spot]) ) {       
	redir_option = args[args_spot++];
	args_spot = isolate_command(args, numargs, args_spot, command2);

	//Command2 will be a command, and piping will occur
	if(!strcmp(redir_option, "|")){
	  piped = 1;
	  pipeline(command, command2);
	  
	  //Save second command of pipe
	  int i = 0;
	  while(pipedcmd[i] = command2[i])
	    i++;
	}

	//Command2 is actually a file, and redirection will occur
	else
	  redirect(redir_option, command2);

      }
      //End of shifting the file table -- semicolon or end of input reached

      if(!piped)
	execute(command);
      else{
	execute(pipedcmd);
	
	//Remove the temporary file used for pipeline
	if(!fork())
	  execlp("/bin/rm", "rm", ".temp", NULL);
	else wait(0);
      }

      //Restore any overwritten standard streams
      dup2(stdout_save, STDOUT_FILENO);
      dup2(stdin_save, STDIN_FILENO);
      dup2(stderror_save, STDERR_FILENO);        
    }
    
  }
}



void execute(char** args){
//Changes directory, exits, or executes a command by searching the PATH
  char* cmd = args[0];

  if( !strncmp(cmd, "cd",2)){
    if( chdir(args[1]) )
      printf("%s: No such file or directory\n", args[1]);
  }
  else if(!strncmp(cmd, "exit", 4))
    exit(0);
  
  //Executes any command other than exit/cd by searching PATH
  else{
    int childexitinfo;
    if(!fork()){
      if(execvp(cmd, args))
	printf("%s: Command not found\n", cmd);
    }
    else
      wait(childexitinfo);
  }

}

void redirect(char* option, char** file){
//Redirects the standard stream indicated by <option> to <file>
  int fd, fdOriginal;

  //FIND STANDARD FILE TO REPLACE, and open the file
  if(!strncmp(option,"<",1)){
    fd = open(file[0], O_RDWR, 0664);
    fdOriginal = STDIN_FILENO;
  }
  else{
    fd = open(file[0], O_RDWR | O_CREAT, 0664);
    if(!strncmp(option,"2>",2))
      fdOriginal = STDERR_FILENO;
    else
      fdOriginal = STDOUT_FILENO;
  }
  
  //LSEEK TO END OF FILE IF APPENDING
  if(!strncmp(option,">>",2))
    lseek(fd, 0, SEEK_END);

  //Save the standard file stream and redirect to the file
  dup2(fd, fdOriginal);
 
}

void pipeline(char** args1, char** args2){
//Redirects cmd1 output into STDIN using an intermediate .temp file 
//Skews ls -a results and deletes any existing .temp file
  int cmd1out = open(".temp", O_RDWR | O_CREAT, 0664);
  int stdout_save = dup(STDOUT_FILENO);
  int stdin_save = dup(STDIN_FILENO);

  //Exec first cmd and print to temp file
  dup2(cmd1out, STDOUT_FILENO);
  execute(args1);
  lseek(cmd1out, 0, SEEK_SET);

  //Set cmd1 output as STDIN and return original stdout
  dup2(cmd1out, STDIN_FILENO);
  dup2(stdout_save, STDOUT_FILENO);

}
