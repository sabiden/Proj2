#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
  while(1){
    char dest[256];
    printf("User:");
    fgets(dest, 256, stdin);
    
    *strchr(dest, '\n')=0;
    
    char *s = dest;
    char * command[100];
    int i;
  
  
    for(i = 0; s; command[i] = strsep( &s, " "), i++);
    command[i]=0;

    int f;
    f = fork();
    if ( !f ){
      execvp(command[0], command);
    }
    else{
      wait();
    }
  }
  return 0;
}
