
#include "my_allocator.h"

#define OPTSTR "b:s:"	// getopt() option list

int convert(int n);

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

	// gets the arguments from the command line and rounds them up to the nearest
	// power of two
	int c;
	while( (c = getopt(argc, argv, "b:s:")) != -1 ){
		switch(c){
			case 'b':
					 b_flag = 1;
					 BASIC_BLOCK_SIZE = convert(atoi(optarg));
					 break;
			case 's':
					 s_flag = 1;
					 MEMORY_LENGTH = convert(atoi(optarg));
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

	// IF 0, no error; IF 1, block memory allocation error; IF 2, free-list allocation error
	unsigned int allocate_flag = init_allocator(BASIC_BLOCK_SIZE, MEMORY_LENGTH);
	switch(allocate_flag){
		case(0):
			printf("Initialization successful.\n");
			break;
		case(1):
			printf("Out of memory. Could not allocate requested amount.\n");
			return 0;
			break;
		case(2):
			printf("Out of memory. Could not allocate free list.\n");
			return 0;
			break;
		
	}
	// Otherwise, run ackerman_main()

	ackerman_main();

	return 0;
}

// rounds n up to the next highest power of two
int convert(int n){
	n = pow(2,ceil(log2(n)));
	return n;
}