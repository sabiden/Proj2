# Proj1
## Our Home Grown Shell!
### Contributers:
- Jack Schluger
- Max Bertfield
- Shaik Abiden

###Features Implemented:
- Runs commands!
- Exits and cd's with ease.
- Run multiple commands on one line with semicolons. 
 - Any number of semicolons can be used.
 - Multiple semicolons in a row (ie: ";;;;") will not cause a crash.
- Any number of spaces can be used to seperate tokens. 
- stdin and stdout redirection. We implemented:
 - >
 - <
 - >>
 - 2>
 - &>
- Pipes with |
- **Any number of pipes can be used with any number of  redirects, in any order!**
- Explanatory prompt that displays current working directory, user, and host name. 
- Readline features:
 - Tab completion
 - Ability to edit command with arrow keys (as opposed to printing the keys to stdin)
 - History of commands, accesable with up and down arrows

###Features we Wanted to Implement, but Couldn't:
- Colors.
- Handeling spaces in file names.
- Tetris easter egg.

###Known Bugs: 
- Cannot do anything with files with spaces in their names.

###Function Headers:
- char * prompt();
- void runcd( char * );
- int finder(char * command[], char * );
- int executeLine( char * );
- int executeCommand( char * );
- void executeFork(char * command[], int );
