#include <stdio.h>

#include "error.h"
#include "util.h"
#include "parser.h"
#include "codegen.h"
#include "a_eval.h"

void print_usage() {
	printf("lcc fatal error: no input files. \n");
}


int compile(char* filename) {
	t_buffer file;
	if (read_file(filename, &file)!=0) {
		raise_error_p1("Could not open file: ",filename);
		return 1;
	}
	parse(&file);

	release_buffer(&file);

	codegen_init();

	codegen_visit(node_root);

	assembler_save("a.asm");

	codegen_validate();

	codegen_cleanup();



	return 0;
}



int main(int argc, char *argv[]) {

//	int val = evaluate_int("10*(5+2)");
//	printf("val: %d\n",val);


	if (argc==0) {
		print_usage();
		return 1;
	}

	if (compile(argv[1])==0)
		printf("ok.\n");


	return 0;
}
