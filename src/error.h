#ifndef __ERROR
#define __ERROR

#include <stdio.h>
#include <stdbool.h>

bool abortAll = false;


void raise_error(char* message);

void raise_error_p1(char* message1, char* message2);




#endif