#include <stdio.h>

#include "util.h"



void print_usage() {
	printf("lcc fatal error: no input files. \n");

}


int compile(char* filename) {
	t_buffer file;
	if (read_file(filename, &file)!=0) {
		raise_error_p1("Could not open file: ",filename);
		return 1;
	}
	printf("%s",file.data);


	release_buffer(&file);
	return 0;
}



int main(int argc, char *argv[]) {

	if (argc==0) {
		print_usage();
		return 1;
	}

	if (compile(argv[1])==0) 
		printf("ok.\n");
	
	return 0;
}