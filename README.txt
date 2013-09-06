The TCKY Shell

Features:
	Executes commands
	Parses and interprets user input strings
	Separate commands by semicolon (extra semicolons will be eradicated)
    	Recursive redirection (Single pipe only)
	Good-lookin' code


Bugs + Additional Info:
     Giving a nonexistent command before > or >> will result in the "command not found" error being printed to the file instead of to the terminal

Files & Function Headers:


     KYCOMMANDLINE.C

	char* read_line();
     	//reads the input from the commandline and returns a string that is the commandline string

     	char** separate_line(char* line);
     	//Separates each argument in line into an array of strings

     	int count_args(char* line);
     	//Returns the number of args in line by counting the number of spaces

     
     PROCESSOR.C
    
	int isRedirectSymbol(char* s);
	//Return 1 if the string is a redirection symbol (checked against an array with symbols)

    	int semicolonFound(char* s);     
	//Returns 1 if the last character of string s is a semicolon
    
	void remove_semicolon(char* s);
     	//turns any semicolons at the end of a string into null

     	void process(char** args, int numargs);
     	//Evaluates the user input given by <args> based on the number of args <numargs>

     	int isolate_command(char** args, int numargs, int arg_spot, char** command);
     	//Isolates the next full command in <args> and inserts it into <command>
     	//Returns the current <args_spot>, which keeps track of the current arg being read in the user input array

     	void execute(char** args);
     	//Changes directories, exits, or executes a command by searching PATH

     	void redirect(char* option, char** file);
     	//Redirects the standard stream indicated by <option> to <file>

     	void pipeline(char** args1, char** args2);
     	//Executes and redirects the output of <args1> into STDIN
     	//<args2> is only there for future implementation of multiple pipelines


     MAIN.C

     	int main()
    	//puts together kycommandline and processor to create a shell
