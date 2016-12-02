#include "main.h"
/*==================================================================================
 char *promt()
 returns information about the users location using getcwd, gethostname and getlogin
 returns string of the hostname, penultimate directory and username
 ==================================================================================*/
char *prompt() {
  char fullcwd[100];
  getcwd( fullcwd, sizeof(fullcwd) );
  
  char *s = fullcwd;
  char *lastDir;
  int i;
  for(i = 0; s; lastDir = strsep( &s, "/"), i++);
  
  char hostName[100];
  hostName[99] = 0;
  gethostname( hostName, sizeof( hostName ) ); 
  s = hostName;
  strsep( &s, ".");
  
  char *user = getlogin();
  char *retString;
  retString = (char *) malloc( 100 );
  sprintf( retString, "%s:%s %s!? ", hostName, lastDir, user );

  return retString;
}
/*==================================================================================
 void runcd( char *target )
 command: takes a target which is a directory path
 runs change directory to move to targer, if target is null it does nothing, if error  it prints an error
 no returns
 ==================================================================================*/
void runcd( char *target ){
  int err;
  if ( target )
    err = chdir( target );
  else
    err = chdir(" ");
  if (err)
    printf("-shell: cd: %s: %s\n", target, strerror( errno ));
}

int finder(char *command[], char *arg ){
  int j = 0;
  for( ; command[j]; j++){
    if(!(strcmp(command[j],arg))){
      return j;
    }
  }
  return 0;
}

/*==================================================================================
 int executeLine( char *in )
 char *in: a line returned from readline, hopefully containng a command
 returns status which is inherited form main and used to exit the shell
 ==================================================================================*/
int executeLine( char *in ) {
  int status = 1;
  char *s = in;
  char * line[15];
  
  int i;
  for(i = 0; (line[i] = strsep( &s, ";")); i++);    
    
  for(i = 0; line[i]; i++) {
    status = status && executeCommand( line[i] );
  }

  return status;
}
/*===========================================================================
 int executeCommand( char *input )
 input: a single command (no ;'s), stored as a char*
 returns 0 on exit, else 1
=============================================================================*/
int executeCommand( char *input ) {
  char *s = input;
  char * command[100];

  if (! strlen(input) ) return 1; 
  int i;
  for(i = 0; (command[i] = strsep( &s, " ")); i++)
    if ( !strlen(command[i]) ) //to get rid of multiple spaces
      i--;

  if (!i) return 1;//if the user only entered spaces
  
  if ( !strcmp("exit",command[0]))
    return 0;
  else if( !(strcmp("cd",command[0])) )
    runcd( command[1] );
  
  else {
    int f;
    f = fork();

    executeFork( command, f );
  }
  return 1;
}
/*==================================================================================
 void executeFork( char *command[] )
 command: an array of strings created by delimiting input to executeCommand by " "
 f: return value from fork() call
 runs the forked command, checks for redirects and then calls executeFork() on whatever doesnt remain
 no returns
 ==================================================================================*/
void executeFork(char *command[], int f) {
  if ( !f ){
      
    int loc;
    //pipe - redirects stdin and out using pipe()
    if( (loc = finder(command,"|")) ) {
      char **command2 = &command[ loc + 1 ];
      command[loc] = 0;
      int fds[2];
      pipe( fds );
	
      int f2;
      f2 = fork();
      if (!f2) {
	close( fds[0] );
	dup2( fds[1], STDOUT_FILENO );
	executeFork( command, f );
      }
      else {
	int stats;
	wait( &stats );
	close( fds[1] );
	dup2( fds[0], STDIN_FILENO );
	executeFork( command2, f );
      }
    }
    //Redirects by copying file into stdout
    else if( (loc = finder(command,">")) ) {
      char *file = command[ loc + 1];
      command[loc] = 0;
      int fd= open(file, O_CREAT | O_RDWR, 0644);
      dup2(fd,STDOUT_FILENO);
      executeFork( command, f );
    }
    //redirects output by copying file into stdout, opens it with append  
    else if( (loc = finder(command,">>")) ) {
      char *file = command[ loc + 1];
      command[loc] = 0;
      int fd= open(file, O_APPEND | O_CREAT | O_RDWR, 0644);
      dup2(fd,STDOUT_FILENO);
      executeFork( command, f );
    }
    //redirects  outputs and errors by copying  file into stdout and stderr
    else if( (loc = finder(command,"&>")) ) {
      char *file = command[ loc + 1];
      command[loc] = 0;
      int fd= open(file, O_CREAT | O_RDWR, 0644);
      dup2(fd,STDOUT_FILENO);
      dup2(fd,STDERR_FILENO);
      executeFork( command, f );
    }
    //redirects only errors by copying file into stderr
    else if( (loc = finder(command,"2>")) ) {
      char *file = command[ loc + 1];
      command[loc] = 0;
      int fd= open(file, O_CREAT | O_RDWR, 0644);
      dup2(fd,STDERR_FILENO);
      executeFork( command, f );
    }
    //redirects input by copying file into Stdin  
    else if( (loc = finder(command,"<")) ) {
      char *file = command[ loc + 1];
      command[loc] = 0;
      int fd = open(file, O_CREAT | O_RDWR, 0644);
      dup2(fd,STDIN_FILENO);
      executeFork( command, f );	
    }
    //theres nothing to redirect so we dont need to recuse anymore
    else{
      execvp(command[0], command);	
    }
  }
  else{
    int stats;
    wait( &stats );
  }
}

int main(){
  int status = 1;
    
  while( status ){
    //get input
    char *pmt = prompt();
    char *in = readline( pmt );

    if ( strlen(in) ) //dont add empty commands to the history
      add_history( in );
    
    status = executeLine( in );
    
  }
  printf(" goodbye\n");
  return 0;
}
