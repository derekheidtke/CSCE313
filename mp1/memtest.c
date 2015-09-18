
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ackerman.h"

#define OPTSTR "b:s:"	// getopt() option list

int main(int argc, char ** argv) {

	// PARSE INPUT ARGUMENTS
	unsigned int BASIC_BLOCK_SIZE;
	unsigned int MEMORY_LENGTH;
	bool b_flag = false,	// Used to provide default argument values if
		 s_flag = false;	// none are given.
	opterr = 0;				// Disable getopt() error message.

	int c;
	while( (c = getopt(argc, argv, "b:s:")) != -1 ){
		switch(c){
			case 'b':
					 b_flag = true;
					 BASIC_BLOCK_SIZE = atoi(optarg);
					 break;
			case 's':
					 s_flag = true;
					 MEMORY_LENGTH = atoi(optarg);
					 break;
		};
	}
	// Provide default arguments if needed.
	if ( !(b_flag) ){
		BASIC_BLOCK_SIZE = 128; // Set default to 128 B
		printf("No argument provided; using default BASIC_BLOCK_SIZE: 128 B\n");
	}
	if ( !(s_flag) ){
		MEMORY_LENGTH = 524288; // Set default to 512 KiB
		printf("No argument provided; using default MEMORY_LENGTH: 512 KiB\n\n");
	}
	
	printf("Basic Block Size: %d B\nMemory Size: %d B\n",
			BASIC_BLOCK_SIZE,MEMORY_LENGTH);
//==============================================================================


	// init_allocator(basic block size, memory length)

	ackerman_main();

	// atexit( release_allocator() ):
}
