
#include "apue.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <unistd.h>

#include "ackerman.h"
#include "my_allocator.h"

#define OPTSTR "b:s:"	// getopt() option list

int main(int argc, char ** argv) {

	// Register release_allocator() to run when program exits normally.
	if( atexit(release_allocator) != 0 )
		err_sys("Can't register release_allocator().\n");

	// PARSE INPUT ARGUMENTS
	unsigned int BASIC_BLOCK_SIZE;
	unsigned int MEMORY_LENGTH;
	int b_flag = 0,		// Used to provide default argument values if
		 s_flag = 0;	// none are given.
	opterr = 0;			// Disable getopt() error message.

	int c;
	while( (c = getopt(argc, argv, "b:s:")) != -1 ){
		switch(c){
			case 'b':
					 b_flag = 1;
					 BASIC_BLOCK_SIZE = atoi(optarg);
					 break;
			case 's':
					 s_flag = 1;
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

	// If no memory allocated, return immediately
	if (init_allocator(BASIC_BLOCK_SIZE, MEMORY_LENGTH) == 1 )
		printf("Out of memory. Could not allocate requested amount.\n");
	// Otherwise, run ackerman_main()

	ackerman_main();

	return 0;
}


