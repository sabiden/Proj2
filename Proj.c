#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void runcd(){
}

int main(){
  while(1){
    char dest[256];
    char cwd[100];
    getcwd(&cwd,sizeof(cwd));
    printf("%s$ ",cwd );
    fgets(dest, 256, stdin);
    
    *strchr(dest, '\n')=0;
    
    char *s = dest;
    char * command[100];
    int i;
    
    for(i = 0; s; command[i] = strsep( &s, " "), i++);
    command[i]=0;

    if(!(strcmp("cd",command[0]))){
      runcd();
    }
    else{
      int f;
      f = fork();
      if ( !f ){
	execvp(command[0], command);
      }
      else{
	wait();
      }
    }
  }
  return 0;
}
