#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdbool.h>

extern bool abort_all;


void raise_error(char* message);

void raise_error_p1(char* message1, char* message2);
void raise_error_p2(char* message1, char* message2, char* message3);




#endif
