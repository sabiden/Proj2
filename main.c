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
    char cwd[100];
    getcwd( cwd, sizeof(cwd) );
    printf("%s$ ",cwd );
    fgets(dest, 256, stdin);
    
    *strchr(dest, '\n')=0;
    
    char *s = dest;
    char * command[100];

    int i;
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
