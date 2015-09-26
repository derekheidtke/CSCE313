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
	unsigned int 			MAGIC = 504649209;	// number to verify that a header is actually a header
/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/
Addr my_malloc(unsigned int _length) {
	// preliminary check:
	// make sure the argument is within given bounds
	if ( _length < 0 || _length > MEM_SIZE ) return 0;
	if ( _length < BLOCK_SIZE ) _length = BLOCK_SIZE;	// round up if too small
	// Now, _length is guaranteed to be within proper range

	Header** FL = FL_MEM;
	Addr address = NULL;

	// find the absolute rank of the block the user wants
	int r = ceil( log2(_length) );	// r is absolute rank.
	int rr = log2(MEM_SIZE)-r;		// rr is relative rank. (ie. the freelist index of the requested block size)

	int i = rr-1;
	while(FL[rr] == NULL){// while the rr tier is empty
		if ( i < 0 ) return 0;		// out of memory
		if(FL[i] == NULL)			// if ith level empty
			i--;						// go up to next level
		else{						// else
			moveDown(i);				// split block in ith level; add two blocks to i+1 level
			i++;						// move back down to split next block if necessary
		}
	}
	// After this, the rr tier should have available space
	// So, give first available block to user
	address =  (Addr)( (int8_t*)(FL[rr]) + (int)sizeof(Header) );	// give user the memory after the header
	// remove given block from free list
	FL[rr] = FL[rr]->NEXT;

	return address;
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

	// Initialize linked list structure
	if ( (FL_MEM = malloc(sizeof(Header*)*MAX_RRANK+1)) == NULL )
		return 2;											// Out of memory error

	Header** free_list = FL_MEM;					// Point free_list to beginning of FL_MEM
	Header* tempHeader = (Header*)MEMORY;			// Point tempHeader to beginning of MEMORY
	tempHeader->NEXT = NULL;						// Initialize values of tempHeader
	tempHeader->size = MEM_SIZE;					//
	tempHeader->MAGIC = MAGIC;						//
	free_list[0] = tempHeader;						// assign tempHeader to first entry in free_list


	// test my_malloc() with MEM_SIZE=128 and BLOCK_SIZE=16
	printf("\nFL_SIZE: %lu\n", sizeof(Header*)*MAX_RRANK+1);
	printf("\nCHUNK_SIZE: %d\n", chunk_size);
	printf("\nSIZEOF_HEADER: %lu\n", sizeof(Header));
	printf("\nMEMORY_START: %p\n", MEMORY);
	Addr* test_arr = malloc(1*sizeof(Addr));
	int n = 0;

	
	n = 15;
	if ( (test_arr[0] = my_malloc(n)) != NULL )
		printf("\nAcquired %d bytes: %p\n", n, test_arr[0]);
	else
		printf("\nFailed.\n");

	n = 32;
	if ( (test_arr[0] = my_malloc(n)) != NULL )
		printf("\nAcquired %d bytes: %p\n", n, test_arr[0]);
	else
		printf("\nFailed.\n");

	n = 32;
	if ( (test_arr[0] = my_malloc(n)) != NULL )
		printf("\nAcquired %d bytes: %p\n", n, test_arr[0]);
	else
		printf("\nFailed.\n");

	n = 32;
	if ( (test_arr[0] = my_malloc(n)) != NULL )
		printf("\nAcquired %d bytes: %p\n", n, test_arr[0]);
	else
		printf("\nFailed.\n");

	n = 15;
	if ( (test_arr[0] = my_malloc(n)) != NULL )
		printf("\nAcquired %d bytes: %p\n", n, test_arr[0]);
	else
		printf("\nFailed.\n");

	n = 2;
	if ( (test_arr[0] = my_malloc(n)) != NULL )
		printf("\nAcquired %d bytes: %p\n", n, test_arr[0]);
	else
		printf("\nFailed.\n");


/*
	// Test the splitting function only
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
	
	return chunk_size;
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
	Header** fl = FL_MEM;				// location of free list
	int m = log2(MEM_SIZE);				// for claculation convenience
	int b = log2(BLOCK_SIZE);			// for claculation convenience

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

	buddy->NEXT = NULL;					// initialize buddy values
	buddy->size = index_h->size;		//
	buddy->MAGIC = MAGIC;				//

	putAfter(index_h,buddy);			// add buddy to (r+1)list

	//printf("\nBUDDYa_AFTER: %p\nSIZE: %d\nNEXT: %p",index_h,index_h->size,index_h->NEXT);
	//printf("\nBUDDYb_AFTER: %p\nSIZE: %d\nNEXT: %p",buddy,buddy->size,buddy->NEXT);

	return 0;
}


int moveUp(int r, Header* current){
	//Header** fl = FL_MEM;
	return 0;
}
