#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "main.h"
#include <fcntl.h>

void prompt() {
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

  printf("%s:%s %s!? ", hostName, lastDir, user );
}

void runcd( char target[] ){
  int err;
  if ( target )
    err = chdir( target );
  else
    err = chdir("~");
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
	 
int execute( char *input ) {
  char *s = input;
  char * command[100];
    
  int i;
  for(i = 0; s; command[i] = strsep( &s, " "), i++);
  command[i]=0;

  if ( !strcmp("exit",command[0]))
    return 0;
  else if( !(strcmp("cd",command[0])) )
    runcd( command[1] );
  
  else{
    int f;
    f = fork();
      
    if ( !f ){
      int loc;
      if( (loc = finder(command,">")) ) {
	char *file = command[ loc + 1];
	command[loc] = 0;
	int fd= open(file, O_CREAT | O_RDWR, 0644);
	dup2(fd,STDOUT_FILENO);
	execvp(command[0], command);
	  
      }
	
      else if( (loc = finder(command,"<")) ) {
	char *file = command[ loc + 1];
	command[loc] = 0;
	int fd= open(file, O_CREAT | O_RDWR, 0644);
	dup2(fd,STDIN_FILENO);
	execvp(command[0], command);
	  
      }
      else{
	execvp(command[0], command);	
      }
    }
    else{
      int stats;
      wait( &stats );
    }
  }
  return 1;
}


int main(){
  int status = 1;
  
  while( status ){
    prompt();

    //get input
    char in[256];
    fgets(in, 256, stdin);
    //get rid of new line
    *strchr(in, '\n') = 0;

    status = execute( in );
  }
  return 0;
}
