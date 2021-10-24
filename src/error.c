#include "error.h"
#include "data.h"
#include <stdlib.h>

bool abort_all = false;


void default_error() {
	printf("\nFatal error on line %d: \n\n",data_current_line);
	abort_all = true;

}

void raise_error(char* message) {
	default_error();
	printf("%s",message);
	printf("\n");
	exit(1);
}

void raise_error_p1(char* message1, char* message2) {
	default_error();
	printf("%s",message1);
	printf("%s",message2);
	printf("\n");
	exit(1);
}

void raise_error_p2(char* message1, char* message2, char* message3) {
	default_error();
	printf("%s",message1);
	printf("%s",message2);
	printf("%s",message3);
	printf("\n");
	exit(1);
}
