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
// #include "apue.h"
// #include <stdlib.h>
// #include <stdio.h>
// #include <math.h>
#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* GLOBAL VARIABLES */ 
/*--------------------------------------------------------------------------*/

	Addr 					MEMORY;	  	// Holds the address of the total aqcuired memory
	Addr 					FL_MEM;		// Holds the address of the free list

	unsigned int 			MEM_SIZE;  	// requested amount of memory
	unsigned int 			BLOCK_SIZE; // requested size of basic blocks
	unsigned int 			LEVELS; 	// CALCULATED: the number of levels in the
										// free list

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/
Addr my_malloc(unsigned int _length) {
	// round up to next power of two

	return malloc(_length);
}

int my_free(Addr _a) {

	free(_a);
	return 0;
}

//============================================================================================

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length) {

	MEM_SIZE   = _length;									// SET GLOBL total mem size variable
	BLOCK_SIZE = _basic_block_size;							// SET GLOBL basic blk size variable
	LEVELS = log2(MEM_SIZE/BLOCK_SIZE)+1;					// Number of ranks

	// malloc the total amount of memory, plus the amount needed for the headers
	int chunk_size = _length + (MEM_SIZE/BLOCK_SIZE)*sizeof(Header*);
	if ( (MEMORY = malloc( chunk_size )) == NULL )
		return 1;											// Out of memory error
	
	// Initialize linked list struct
	if ( (FL_MEM = malloc(sizeof(Header*)*LEVELS)) == NULL )
		return 2;											// Out of memory error

	Header** free_list = FL_MEM;			// Point free_list to beginning of FL_MEM
	Header* tempHeader = (Header*)MEMORY;	// Point tempHeader to beginning of MEMORY
	tempHeader->NEXT = NULL;				// Initialize values of tempHeader
	tempHeader->size = MEM_SIZE;			// "                               "
	free_list[0] = tempHeader;				// assign tempHeader to first entry in free_list

	return 0;
}

void release_allocator(void) {
	// Free MEMORY
	free(MEMORY);
	free(FL_MEM);
}
