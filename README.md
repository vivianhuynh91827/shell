# Shell
Vivian Huynh and Jeffrey Chen Period 5

### Features:
- Runs all commands
- Run multiple commands in one line separated by semi-colons
- Implements redirection and pipes
	
### Attempted:
- We tried to implement multiple pipes

### Bugs:
- If pipe is given an invalid input, there will be an error and the shell crashes
- Redirect and pipe are not able to run properly if the input on one side has more than 1 argument

## Files & Function Headers:
### execute.c
	void exec_line( char * line );
Returns: Nothing
  
Calls parse_args in order to discern commands seperated by semicolons and then calls exec_command
  
  	void exec_command( char * command );
Returns: Nothing
  
Calls parse_args in order to discern arguments and continues calling execute functions until it reaches the correct one

If the correct exec command is called, it will return 1, causing exec_command to return

Otherwise, it continues trying the other execute functions
	
	int exec_cd( char ** args);
Returns: 1 if cd is the first argument, 0 otherwise
	
Executes cd

	int exec.exit( char ** args );
Returns: 0 if first argument is NOT "exit"

Executes exit

	void exec_regular( char ** args );
Returns: Nothing

Executes a regular command. Prints an error message if command is invalid

	int exec_double_redirect( char ** args );
Returns: 1 if the command calls double redirection, 0 otherwise

Executes double redirection
  
There are two cases accounted for:
- Redirecting output twice e.g. ls -l > output > output2
- Redirecting input and then output e.g. tr a-z A-Z < output > output2.
```	
	int exec_redirect_output( char ** args );
```
Returns: 1 if redirect output is called, 0 otherwise

Executes simple redirection using > (redirecting stdout)

	void output( char ** input, char * output);
Returns: Nothing

Executes the commands in input and outputs it to output

	int exec_redirect_input( char ** args );
Returns: 1 if redirect input is called, 0 otherwise

Executes simple redirection using < (redirecting stdin)

	int exec_pipe( char ** args );
Returns: 1 if pipe is called, 0 otherwise

Executes pipe command

### parse.c
	char ** parse_args( char * line, char * delim );
Returns: Double pointer to parts of the line separated by delim

Parses a command line into its arguments

	char * strip( char * line );
Returns: line without any leading or trailing white space

	int len_args( char ** args );
Returns: the number of arguments

	int count_redirects( char ** args );
Returns: the number of occurences of > or <

	int count_redirect_in( char ** args);
Returns: the number of occurences of > 
