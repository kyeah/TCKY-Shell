char* read_line();
//reads the input from the commandline and returns a string that is the commandline string

char** separate_line(char* line);
//Separates each argument in line into an array of strings

int count_args(char* line);
//Returns the number of args in line by counting the number of spaces
