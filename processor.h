int isRedirectSymbol(char* s);
//if the string is a redirection symbol(checked against an array with symbols) then it returns 1, otherwise returns 0

int semicolonFound(char* s);     
//if the last char in a string is a ";" then it returns 1, otherwise returns 0

void remove_semicolon(char* s);
//turns any semicolons at the end of a string into a null

void process(char** args, int numargs);
//Evaluates the user input given by <args> based on the number of args <numargs>

int isolate_command(char** args, int numargs, int arg_spot, char** command);
//Isolates the next full command in <args> and inserts it into <command>
//Returns the current <args_spot>, which keeps track of the current arg being read in the user input array

void execute(char** args);
//executes a command by changing directories or exiting, otherwise searches the PATH

void redirect(char* option, char** file);
//Redirects the standard stream indicated by <option> to <file>

void pipeline(char** args1, char** args2);
//Executes and redirects the output of <args1> into STDIN
//<args2> is only there for future implementation of multiple pipelines
