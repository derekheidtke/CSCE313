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
#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

	struct free_list
	{
		Addr HEAD, TAIL;
	};

/*--------------------------------------------------------------------------*/
/* GLOBAL VARIABLES */ 
/*--------------------------------------------------------------------------*/

	Addr 			MEMORY;	  // Holds the address of the aqcuired memory
	unsigned int 	MEM_SIZE;  // size of acquired memory

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
	return malloc((size_t)_length);
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
	   from the runtime system.
	*/
	if ( (MEMORY = malloc( _length )) == NULL )
		return -1;			// Out of memory error
	MEM_SIZE = _length;		// Set length of memory if allocation successful.
	printf("=====MEMORY ALLOCATED====\n");
	/*	  
	   Initialize linked list struct
	*/
	return 0;
}

void release_allocator(void) {
	// Free MEMORY
	free(MEMORY);
	printf("=====MEMORY FREED====\n");
}
