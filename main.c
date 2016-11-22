#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "main.h"

void runcd( char target[] ){
  int err;
  if ( !strcmp(target,"") )
    err = chdir("~");
  else
    err = chdir( target );

  if (err)
    printf("-shell: cd: %s: %s\n", target, strerror( errno ));
}

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

int main(){

  while(1){
    prompt();
    
    char dest[256];
    fgets(dest, 256, stdin);
    
    *strchr(dest, '\n') = 0;
    
    char *s = dest;
    char * command[100];
    
    int i;
    for(i = 0; s; command[i] = strsep( &s, " "), i++);
    command[i]=0;
    
    if( !(strcmp("cd",command[0])) ){
      if ( command[1] )
	runcd( command[1] );
      else
	runcd( "" );
    }
    else{
      int f;
      f = fork();
      if ( !f ){
	execvp(command[0], command);
      }
      else{
	int stats;
	wait( &stats );
      }
    }
  }
  return 0;
}
