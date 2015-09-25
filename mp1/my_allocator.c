/* 
	File: my_allocator.c

	Author: <your name>
			Department of Computer Science
			Texas A&M University
	Date	: <date>

	Modified: 

	This file contains the implementation of the module "MY_ALLOCATOR".

*/

#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* GLOBAL VARIABLES */ 
/*--------------------------------------------------------------------------*/

	Addr 					MEMORY;	  	// Holds the address of the total aqcuired memory
	Addr 					FL_MEM;		// Holds the address of the free list

	unsigned int 			MEM_SIZE;  	// requested amount of memory
	unsigned int 			BLOCK_SIZE; // requested size of basic blocks
	unsigned int 			MAX_RRANK; 	// CALCULATED: the number of levels in the
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
	MAX_RRANK = log2(MEM_SIZE/BLOCK_SIZE)+1;					// Number of ranks

	// malloc the total amount of memory, plus the amount needed for the headers
	int chunk_size = _length + (MEM_SIZE/BLOCK_SIZE)*sizeof(Header*);
	if ( (MEMORY = malloc( chunk_size )) == NULL )
		return 1;											// Out of memory error

	// Initialize linked list struct
	if ( (FL_MEM = malloc(sizeof(Header*)*MAX_RRANK)) == NULL )
		return 2;											// Out of memory error

	Header** free_list = FL_MEM;					// Point free_list to beginning of FL_MEM
	Header* tempHeader = (Header*)MEMORY;			// Point tempHeader to beginning of MEMORY
	tempHeader->NEXT = NULL;						// Initialize values of tempHeader
	tempHeader->size = MEM_SIZE;					// "	                           "
	free_list[0] = tempHeader;						// assign tempHeader to first entry in free_list

	return 0;
}

void release_allocator(void) {
	// Free MEMORY
	free(MEMORY);
	free(FL_MEM);
}

// ==============================================================================================
// HELPER FUNCTIONS

int putAfter(Header* current, Header* hp){
	if ( current == NULL || hp == NULL ) return 1;

	if( current->NEXT == NULL ){ 						// if appending list
		current->NEXT = hp;
		hp->NEXT = NULL;
	}else{											// insert to middle of list
		Header* temp = current->NEXT;
		current->NEXT = hp;
		hp->NEXT = temp;
	}
	return 0;
}

int removeNext(Header* current){
	if( current == NULL ) return 1;			// error if passed NULL argument

	if ( current->NEXT == NULL ) return 2;	// error if current is last

	Header* temp = current->NEXT->NEXT;
	current->NEXT->NEXT = NULL;
	current->NEXT = temp;

	return 0;
}
int moveDown(int r, Header* current){
	Header** fl = FL_MEM;

	// Make sure r isn't last level (ie. basicBlockSize).
	if ( r < 0 || r >= MAX_RRANK ) return 1;					// r is out of range
	// Verify correct size
	if ( -1*log2(current->size)+log2(MEM_SIZE) != r) return 2;	// current size doesn't match given r
	// check if tier is empty
	if ( fl[r] == NULL ) return 3;						// can't move anything if tier is empty

	Header* index_h = fl[r];

	// look for current in r-list
	if( index_h != current ){					// if current is not first in list
		while( index_h->NEXT != NULL ){			// check the rest of the list
			if( index_h->NEXT == current ) break;

			index_h = index_h->NEXT;			// increment index
		}										// after loop, index_h will be previous current
		removeNext( index_h );					// remove Header after index_h (ie. current)
	}else{										// current is first; redirect freelist pointer
		fl[r] = current->NEXT;
	}
	// add current to (r+1)-list
	index_h = fl[r+1];
	if(index_h->NEXT == NULL){
		while(index_h->NEXT != NULL){
			index_h = index_h->NEXT;
		}
	}
	putAfter(index_h, current);

	return 0;
}
int moveUp(int r, Header* current){
	Header** fl = FL_MEM;
	return 0;
}
