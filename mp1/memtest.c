
#include "my_allocator.h"

#define OPTSTR "b:s:o"	// getopt() option list

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
	_Bool o_flag = 0;
	opterr = 0;			// Disable getopt() error message.

	// gets the arguments from the command line and rounds them up to the nearest
	// power of two
	int c;
	set_output_flag(0);
	while( (c = getopt(argc, argv, OPTSTR)) != -1 ){
		switch(c){
			case 'b':
					 b_flag = 1;
					 BASIC_BLOCK_SIZE = convert(atoi(optarg));
					 break;
			case 's':
					 s_flag = 1;
					 MEMORY_LENGTH = convert(atoi(optarg));
					 break;
			case 'o':
					 set_output_flag(1);
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

	// initialize the allocator
	unsigned int allocate_flag = init_allocator(BASIC_BLOCK_SIZE, MEMORY_LENGTH);

	if(allocate_flag == 0){
		printf("\nAllocator initialization error.");
		return 0;
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