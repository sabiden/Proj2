#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "readline/readline.h"
#include "readline/history.h"

char *prompt();

void runcd( char * );

int finder(char *command[], char * );

int executeLine( char * );

int executeCommand( char *);

void executeFork(char *command[], int );
