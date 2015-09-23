/* 
	File: my_allocator.c

	Author: <your name>
			Department of Computer Science
			Texas A&M University
	Date	: <date>

	Modified: 

	This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* GLOBAL VARIABLES */ 
/*--------------------------------------------------------------------------*/

	Addr 					MEMORY;	  	// Holds the address of the aqcuired memory
	unsigned int 			MEM_SIZE;  	// size of acquired memory
	unsigned int 			BLOCK_SIZE; 
	Addr 					FL_MEM;

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/
Addr my_malloc(unsigned int _length) {
	/* This preliminary implementation simply hands the call over the 
	   the C standard library! 
	   Of course this needs to be replaced by your implementation.

	   Use the buddy system to get the memories.
	   Break large chunks into smaller ones.
	*/
	  // round up to nect power of two
	Addr rqst_mem;
	
	//return rqst_mem+COUNT*64;
	return malloc(_length);
}

int my_free(Addr _a) {
	/*
	   Free the address given.
	   Coalesce 
	*/

	free(_a);
	return 0;
}

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length) {
	/* Use the malloc() function to request the correct amount of memory
	   from the runtime system.*/
	if ( (MEMORY = malloc( _length )) == NULL )
		return 1;			// Out of memory error

	MEM_SIZE   = _length;		// Set length of memory if allocation successful.
	BLOCK_SIZE = _basic_block_size;

	printf("\n>>>>>>>>>>>>>>>>>>>>>>MEMORY>ALLOCATED>>>>>>>>>>>>>>>>>>>>>>\n");
	/*	  
	   Initialize linked list struct
	*/


	// printf("ADDRESS_OF_FL: %p\n", &FL);
	// printf("SIZE_OF_FL: %lu\n", sizeof(FL));

	return 0;
}

void release_allocator(void) {
	// Free MEMORY
	free(MEMORY);
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<MEMORY<FREED<<<<<<<<<<<<<<<<<<<<<<<\n\n");
}
