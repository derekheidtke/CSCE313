
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ackerman.h"

#define OPTSTR "b:s:"

int main(int argc, char ** argv) {

	// PARSE INPUT ARGUMENTS
	// input parameters (basic block size, memory length)
	int BASIC_BLOCK_SIZE;
	int MEMORY_LENGTH;
	int c;
	opterr = 0;	

	while( (c = getopt(argc, argv, "b:s:")) != -1 ){
		switch(c){
			case 'b':
					 BASIC_BLOCK_SIZE = atoi(optarg);
					 break;
			case 's':
					 MEMORY_LENGTH = atoi(optarg);
					 break;
		};
	}
	printf("Basic Block Size: %d\nMemory Size: %d\n",
			BASIC_BLOCK_SIZE,MEMORY_LENGTH);
//==============================================================================


	// init_allocator(basic block size, memory length)

	ackerman_main();

	// atexit( release_allocator() ):
}
