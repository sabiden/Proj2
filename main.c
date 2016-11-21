#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "main.h"

void runcd( char target[] ){
  int err;
  err = chdir( target );
  if (err)
    printf("-shell: cd: %s: %s\n", target, strerror( errno ));
}

int main(){

  while(1){
    char dest[256];
    char fullcwd[100];
    getcwd( fullcwd, sizeof(fullcwd) );

    char *s = fullcwd;
    char *lastDir;
    int i;
    for(i = 0; s; lastDir = strsep( &s, "/"), i++);

    printf("machine:%s user$ ", lastDir );
    fgets(dest, 256, stdin);
    
    *strchr(dest, '\n')=0;
    
    s = dest;
    char * command[100];

    for(i = 0; s; command[i] = strsep( &s, " "), i++);
    command[i]=0;
    
    if(!(strcmp("cd",command[0]))){
      runcd( command[1] );
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
