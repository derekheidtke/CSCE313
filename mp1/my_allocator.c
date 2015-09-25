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
	MAX_RRANK = log2(MEM_SIZE/BLOCK_SIZE);					// Number of ranks

	// malloc the total amount of memory, plus the amount needed for the headers
	int chunk_size = _length + (MEM_SIZE/BLOCK_SIZE)*sizeof(Header);
	if ( (MEMORY = malloc( chunk_size )) == NULL )
		return 1;											// Out of memory error

	// Initialize linked list struct
	if ( (FL_MEM = malloc(sizeof(Header*)*MAX_RRANK+1)) == NULL )
		return 2;											// Out of memory error

	Header** free_list = FL_MEM;					// Point free_list to beginning of FL_MEM
	Header* tempHeader = (Header*)MEMORY;			// Point tempHeader to beginning of MEMORY
	tempHeader->NEXT = NULL;						// Initialize values of tempHeader
	tempHeader->size = MEM_SIZE;					// "	                           "
	free_list[0] = tempHeader;						// assign tempHeader to first entry in free_list

	/*
	// test with MEM_SIZE=128 and BLOCK_SIZE=16
	printf("\nFL_SIZE: %d\n", sizeof(Header*)*MAX_RRANK+1);
	printf("\nCHUNK_SIZE: %d\n", chunk_size);
	printf("\nSIZEOF_HEADER: %d\n", sizeof(Header));
	printf("\n\n");

	printf("SPLIT_RANK_0\n");
	moveDown(0);
	printf("\n\n");

	printf("SPLIT_RANK_1\n");
	moveDown(1);
	printf("SPLIT_RANK_1\n");
	moveDown(1);
	printf("SPLIT_RANK_1(shouldn't work)\n");
	moveDown(1);
	printf("\n\n");

	printf("SPLIT_RANK_2\n");
	moveDown(2);
	printf("SPLIT_RANK_2\n");
	moveDown(2);
	printf("SPLIT_RANK_2\n");
	moveDown(2);
	printf("SPLIT_RANK_2\n");
	moveDown(2);
	printf("SPLIT_RANK_2(shouldn't work)\n");
	moveDown(2);
	printf("\n\n");

	printf("SPLIT_RANK_3(shouldn't work)\n");
	moveDown(3);
	printf("\n\n");
*/
	
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

	Header* temp = ( (Header*)(current->NEXT) )->NEXT;
	( (Header*)(current->NEXT) )->NEXT = NULL;
	current->NEXT = temp;

	return 0;
}

// Find first free r-block,
// move it to the (r+1)-list and create its buddy
int moveDown(int r){
	Header** fl = FL_MEM;
	int m = log2(MEM_SIZE);
	int b = log2(BLOCK_SIZE);

	// Make sure r isn't last level (ie. basicBlockSize).
	if ( r < 0 || r >= MAX_RRANK ){
		printf("\nSPLIT ERROR: r less than 0, or r greater than MAX_RRANK.\n\n");
		return 1;					// r is out of range
	}

	// check if tier is empty
	if ( fl[r] == NULL ) {
		printf("\nEMPTY TIER: tier of rank %d, is empty. Cannot split block from empty list.\n\n", r);
		return 3;						// can't move anything if tier is empty
	}

	Header* index_h = fl[r];		// index_h set to point to first r-header
	//printf("\nBUDDYa_BEFORE: %p\nSIZE: %d\nNEXT: %p",index_h,index_h->size,index_h->NEXT);

	// remove index_h from r-list; **works when tier has one entry
	fl[r] = index_h->NEXT;
	// add index_h to (r+1)-list; adjust size to match new tier
	index_h->NEXT = fl[r+1];
	fl[r+1] = index_h;
	index_h->size = pow( 2, m-r-1 );

	// create buddy
	int8_t* temp_buddy = (int8_t*)index_h + (int)pow( 2, m-r-1 ) + (int)(sizeof(Header)*pow( 2, m-b-r-1 ));
	Header* buddy = (Addr)temp_buddy;
	buddy->NEXT = NULL;
	buddy->size = index_h->size;
	// add buddy to (r+1)list
	putAfter(index_h,buddy);

	//printf("\nBUDDYa_AFTER: %p\nSIZE: %d\nNEXT: %p",index_h,index_h->size,index_h->NEXT);
	//printf("\nBUDDYb_AFTER: %p\nSIZE: %d\nNEXT: %p",buddy,buddy->size,buddy->NEXT);

	return 0;
}


int moveUp(int r, Header* current){
	//Header** fl = FL_MEM;
	return 0;
}
