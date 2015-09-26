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

#define _MALLOC_FREE 1

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
	#if _MALLOC_FREE == 0
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
			split(i);				// split block in ith level; add two blocks to i+1 level
			i++;						// move back down to split next block if necessary
		}
	}
	// After this, the rr tier should have available space
	// So, give first available block to user
	address =  (Addr)( (int8_t*)(FL[rr]) + (int)sizeof(Header) );	// give user the memory after the header
	// remove given block from free list
	FL[rr] = FL[rr]->NEXT;

	return address;
	#else
	return malloc(_length);
	#endif
}

int my_free(Addr _a) {
	#if _MALLOC_FREE == 0
	// invalid argument
	if( _a == NULL ) return 1;

	Header** FL = FL_MEM;

	// find associated header
	int8_t* user_start = _a;
	Header* header_start = (Header*)( (void*)(user_start - (int)sizeof(Header)) );

	// make sure header has magic number
	if ( header_start->MAGIC != MAGIC ) return 2;

	// get relative rank from header size
	int rrank = log2(MEM_SIZE)-log2(header_start->SIZE);

	// add header back onto free list
	header_start->NEXT = FL[rrank];
	FL[rrank] = header_start;		// added to the beginning

	// find buddy and join if possible
	int i = rrank;
	Header* ih = header_start;
	while( i >= 0 ){
		while( (ih = join(ih)) != NULL ){}
		i--;
	} // After this loop, all available buddy pairs should have been joined

	#else
	free(_a);
	#endif

	return 0;		// free was successful
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
	tempHeader->SIZE = MEM_SIZE;					//
	tempHeader->MAGIC = MAGIC;						//
	tempHeader->BUDDY = 'A';						//
	free_list[0] = tempHeader;						// assign tempHeader to first entry in free_list

/*
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
*/

/*
	// Test the splitting function only
	// test with MEM_SIZE=128 and BLOCK_SIZE=16
	printf("\nFL_SIZE: %d\n", sizeof(Header*)*MAX_RRANK+1);
	printf("\nCHUNK_SIZE: %d\n", chunk_size);
	printf("\nSIZEOF_HEADER: %d\n", sizeof(Header));
	printf("\n\n");

	printf("SPLIT_RANK_0\n");
	split(0);
	printf("\n\n");

	printf("SPLIT_RANK_1\n");
	split(1);
	printf("SPLIT_RANK_1\n");
	split(1);
	printf("SPLIT_RANK_1(shouldn't work)\n");
	split(1);
	printf("\n\n");

	printf("SPLIT_RANK_2\n");
	split(2);
	printf("SPLIT_RANK_2\n");
	split(2);
	printf("SPLIT_RANK_2\n");
	split(2);
	printf("SPLIT_RANK_2\n");
	split(2);
	printf("SPLIT_RANK_2(shouldn't work)\n");
	split(2);
	printf("\n\n");

	printf("SPLIT_RANK_3(shouldn't work)\n");
	split(3);
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
int split(int r){
	Header** FL = FL_MEM;				// location of free list
	int m = log2(MEM_SIZE);				// for claculation convenience
	int b = log2(BLOCK_SIZE);			// for claculation convenience

	// Make sure r isn't last level (ie. basicBlockSize).
	if ( r < 0 || r >= MAX_RRANK ){
		printf("\nSPLIT ERROR: r less than 0, or r greater than MAX_RRANK.\n\n");
		return 1;					// r is out of range
	}
	// check if tier is empty
	if ( FL[r] == NULL ) {
		printf("\nEMPTY TIER: tier of rank %d, is empty. Cannot split block from empty list.\n\n", r);
		return 3;						// can't move anything if tier is empty
	}

	Header* index_h = FL[r];		// index_h set to point to first r-header
	//printf("\nBUDDYa_BEFORE: %p\nSIZE: %d\nNEXT: %p",index_h,index_h->size,index_h->NEXT);

	// remove index_h from r-list; **works when tier has one entry
	FL[r] = index_h->NEXT;
	// add index_h to (r+1)-list; adjust size to match new tier
	index_h->NEXT = FL[r+1];
	FL[r+1] = index_h;
	index_h->SIZE = pow( 2, m-r-1 );


	// make index_h into 'A' buddy
	index_h->BUDDY = 'A';			// becomes 'A' buddy in new list regardless of previous value
	// create buddy
	int8_t* temp_buddy = (int8_t*)index_h + (int)pow( 2, m-r-1 ) + (int)(sizeof(Header)*pow( 2, m-b-r-1 ));
	Header* buddy = (Addr)temp_buddy;

	buddy->NEXT = NULL;					// initialize buddy values
	buddy->SIZE = index_h->SIZE;		//
	buddy->MAGIC = MAGIC;				//
	buddy->BUDDY = 'B';					// created buddy is always 'B'

	putAfter(index_h,buddy);			// add buddy to (r+1)list

	//printf("\nBUDDYa_AFTER: %p\nSIZE: %d\nNEXT: %p",index_h,index_h->size,index_h->NEXT);
	//printf("\nBUDDYb_AFTER: %p\nSIZE: %d\nNEXT: %p",buddy,buddy->size,buddy->NEXT);

	return 0;
}

// takes pointer to one buddy
// returns address of joined buddies, NULL if unsuccessful
Header* join(Header* buddy){
	Header** FL = FL_MEM;

	if( buddy == NULL ) return NULL;		// buddy should not be NULL

	// get rr from size, and first_buddy from BUDDY
	int rr = log2(MEM_SIZE) - log2( buddy->SIZE );
	if( FL[rr] == NULL ) return NULL;		// FL[rr] should not be empty
	if( rr <= 0 ) return NULL;					// can't join zeroth tier or negative

	char first_buddy = buddy->BUDDY;
	// char second_buddy = ( first_buddy == 'A' ? 'B' : 'A' );

	// find the appropriate offset address for the given buddy
	// base+sizeof(Header) if 'A' buddy; base-sizeof(Header) if 'B' buddy
	Addr offset = NULL;
	if( first_buddy == 'A' )
		offset = (void*)((int8_t*)buddy + sizeof(Header));
	else if( first_buddy == 'B' )
		offset = (void*)((int8_t*)buddy - sizeof(Header));
	else
		return NULL; 						// buddy labeling error

	Header* tmp2 = FL[rr];			// initialized to first in rr-list
	Header* tmp1 = NULL;
	Header* match = NULL;
	_Bool found = 0;

	// FIND buddy's match:
	// check tmp2
	if( tmp2 == offset ){			// if match is first in free list
		found = 1;						// set found flag
		match = tmp2;					// identify match
	}else{							// go on to check the second
		tmp1 = tmp2->NEXT;				// tmp1 will always be after tmp2
		if (tmp1 == NULL) return NULL;	// make sure a second entry exists
	}

	// check tmp1
	if( !found && tmp1->NEXT == offset ){	// if match is second in free list
		found = 1;									// set found flag
		match = tmp1;								// identify match
	}

	// do this if not found in first or second entry:
	// iterate through linked list to find buddy's match
	while( !found && tmp1->NEXT != NULL ){ 	// loop while !(found or at_end)
		if( tmp1 == offset ){					// if offset matches free block
			found = 1;								// set found flag
			match = tmp1;							// identify match
			break;
		}
		tmp1 = tmp1->NEXT;		// increment both pointers
		tmp2 = tmp2->NEXT;		//
	} 	
	if( match == NULL ) return NULL;
		// After this, match should be found (if it exists)
		// and tmp2 should be the header immediately before the match.
		// First buddy should always be first in the list.

	// remove both buddies from free list and add 'A' buddy to (r-1) list
	if( removeNext(tmp2) != 0 )	// remove the match
		return NULL;			//

	FL[rr] = buddy->NEXT;		// remove first buddy

	// determine which buddy is which
	Header* A = ( first_buddy == 'A' ? buddy : match );
	Header* B = ( first_buddy == 'B' ? buddy : match );
	if( A->BUDDY == B->BUDDY ) return NULL;		// should not happen

	// add A buddy to (rr-1) list
	Header* temp = FL[rr-1];
	FL[rr-1] = A;
	A->NEXT = temp;

	int AOrB = AorB( (Addr)A, rr-1 );
	if     ( AOrB == 0 )
		return NULL;					// unsuccessful
	else if( AOrB == 1 )
		A->BUDDY = 'A';
	else if( AOrB == 2 )
		A->BUDDY = 'B';

	return A;
}

// Determines whether a header is associated with an A or B buddy
// when given the memory tier, rr.
// Returns 1 for 'A,' or 2 for 'B,' and 0 if unsuccessful.
int AorB(Addr header, int rr){
	return 0;
}