#include "error.h"


void raise_error(char* message) {
	printf("Terminal error: \n\n");
	printf("%s",message);
	printf("\n");
	abortAll = false;
}

void raise_error_p1(char* message1, char* message2) {
	printf("Terminal error: \n\n");
	printf("%s",message1);
	printf("%s",message2);
	printf("\n");
	abortAll = true;
}

