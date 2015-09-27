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

#define _MALLOC_FREE 0		// training wheels for testing and data verification

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
												// Used during my_free().
/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/
Addr my_malloc(unsigned int _length) {
	#if _MALLOC_FREE == 0
	// preliminary check:
	// make sure the argument is within given bounds
	if ( _length < 0 || _length > MEM_SIZE ){
		printf("\nERROR IN MY_MALLOC(): USER REQUESTED INVALID MEMORY LENGTH: %u\n", _length);
		return 0;
	}
	if ( _length < BLOCK_SIZE ) _length = BLOCK_SIZE;	// round up if too small
	// Now, _length is guaranteed to be within proper range

	Header** FL = FL_MEM;
	Addr address = NULL;

	// find the absolute rank of the block the user wants
	int r = ceil( log2(_length) );	// r is absolute rank.
	int rr = log2(MEM_SIZE)-r;		// rr is relative rank. (ie. the freelist index of the requested block size)

	int i = rr-1;
	// printf("\nREL_RANK(ABS): %d(%d)", rr,(int)(log2(MEM_SIZE)-rr) );
	Header* nexth = NULL;

	// find closet available rank to requested rank
	while(1){
		if( i < 0 ){
			printf("\nERROR IN MY_MALLOC(): NO AVAILABLE BLOCKS ON FREELIST (OUT OF MEMORY).");
			printf("\nREQUESTED: %6x(%6x)\n",_length,(int)pow(2,r));
			list_lists();
			return NULL;
		}
		if(FL[i] != NULL){		// if ith list is not empty
			break;					// break and don't change i
		}
		i--;					// go to next lower level
	}
	// split until requested rank is available
	nexth = FL[i];
	while(FL[rr] == NULL){		// while requested level is empty
		nexth = split( log2( MEM_SIZE/(nexth->SIZE) ) );	// split
	}


	// while(FL[rr] == NULL){				// while the rr tier is empty
	// 		printf("\nINDEX: %d", i);
	// 	if ( i < 0 ){
	// 		printf("\nERROR IN MY_MALLOC(): INDEX BECAME NEGATIVE (OUT OF MEMORY).\n");
	// 		return 0;		// out of memory
	// 	}
	// 	if(FL[i] == NULL)			// if ith level empty
	// 		i--;						// go up to next level
	// 	else{						// else
	// 		split(i);				// split block in ith level; add two blocks to i+1 level
	// 		i++;						// move back down to split next block if necessary
	// 	}
	// }


	// After this, the rr tier should have available space
	// So, give first available block to user
	address =  (Addr)( (int8_t*)(FL[rr]) + (int)sizeof(Header) );	// give user the memory after the header
	// remove given block from free list
	FL[rr] = FL[rr]->NEXT;

		if (address == NULL){
			printf("\nMY_MALLOC() RETURNED A NULL.");
		}
		return address;
	#else
		return malloc(_length);
	#endif
}

int my_free(Addr _a) {
	#if _MALLOC_FREE == 0
	// invalid argument
	if( _a == NULL ){
		printf("\nERROR IN MY_FREE(): USER PROVIDED NULL* FOR FREEING.\n");
		return 1;
	}

	Header** FL = FL_MEM;

	// find associated header
	int8_t* user_start = _a;
	Header* header_start = (Header*)( (void*)(user_start - (int)sizeof(Header)) );

	// make sure header has magic number
	if ( header_start->MAGIC != MAGIC ){
		printf("\nERROR IN MY_FREE(): ALIGNMENT ERROR. MAGIC NUMBER DOES NOT MATCH.\n");
		return 2;
	}

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
		if(ih == NULL) break;
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
	if ( (MEMORY = malloc( chunk_size )) == NULL ){
		printf("\nERROR IN INIT_ALLOCATOR(): REQUESTED MEMORY UNABLE TO BE ALLOCATED.\n");
		return 1;											// Out of memory error
	}

	// Initialize linked list structure
	int free_list_size = sizeof(Header*)*(MAX_RRANK+1);
	if ( (FL_MEM = malloc(free_list_size)) == NULL ){
		printf("\nERROR IN INIT_ALLOCATOR(): FREELIST UNABLE TO BE ALLOCATED.\n");
		return 2;											// Out of memory error
	}

	Header** free_list = FL_MEM;					// Point free_list to beginning of FL_MEM
	Header* tempHeader = (Header*)MEMORY;			// Point tempHeader to beginning of MEMORY
	tempHeader->NEXT = NULL;						// Initialize values of tempHeader
	tempHeader->SIZE = MEM_SIZE;					//
	tempHeader->MAGIC = MAGIC;						//
	tempHeader->BUDDY = 'A';						//
	free_list[0] = tempHeader;						// assign tempHeader to first entry in free_list

	// test my_malloc() with MEM_SIZE=128 and BLOCK_SIZE=16
	printf("\nFL_SIZE: %lu", sizeof(Header*)*(MAX_RRANK+1));
	printf("\nCHUNK_SIZE: %d", chunk_size);
	printf("\nSIZEOF_HEADER: %lu", sizeof(Header));
	printf("\nSIZEOF_HEADER*: %lu", sizeof(Header*));
	printf("\nMEMORY_START: %p", MEMORY);
/*
	Header* tmph[20];
	Addr    tmpa[20];
	list_lists();
	split(0);
	list_lists();
	split(1);
	split(1);
	list_lists();
	tmph[0] = split(2);
	tmph[1] = tmph[0]->NEXT;
	tmph[2] = split(2);
	tmph[3] = tmph[2]->NEXT;
	tmph[4] = split(2);
	tmph[5] = tmph[4]->NEXT;
	tmph[6] = split(2);
	tmph[7] = tmph[6]->NEXT;
	list_lists();

	char buddy[10];
	int AOrB[10];
	for(int i = 0; i < 8 ; i++){
		AOrB[i] = AorB(tmph[i],3);
		if     ( AOrB[i] == 0 )
			printf("\nUNSUCCESSFUL");					// unsuccessful
		else if( AOrB[i] == 1 )
			buddy[i] = 'A';
		else if( AOrB[i] == 2 )
			buddy[i] = 'B';
		printf("\nHEADER_ADDR: %15p   BUDDINESS: %2c",tmph[i],tmph[i]->BUDDY);
	}
	list_lists();
*/


/*
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
	if ( current == NULL || hp == NULL ){
		printf("\nERROR IN PUTAFTER(): PASSED NULL* FOR ONE OR TWO ARGUMENTS.\n");
		return 1;
	}

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
	if( current == NULL ){
		printf("\nERROR IN REMOVENEXT(): PASSED A NULL* AS ARGUMENT.\n");
		return 1;			// error if passed NULL argument
	}

	if ( current->NEXT == NULL ){
		printf("\nERROR IN REMOVENEXT(): PASSED HEADER HAS NO NEXT HEADER.\n");
		return 2;	// error if current is last
	}

	Header* temp = ( (Header*)(current->NEXT) )->NEXT;
	( (Header*)(current->NEXT) )->NEXT = NULL;
	current->NEXT = temp;

	return 0;
}

// Find first free r-block,
// move it to the (r+1)-list and create its buddy
// return address of split 'A' buddy, NULL if error
Header* split(int r){
	Header** FL = FL_MEM;				// location of free list
	int m = log2(MEM_SIZE);				// for claculation convenience
	int b = log2(BLOCK_SIZE);			// for claculation convenience
	int r_block_size = pow( 2, m-r-1 ) + (int)(sizeof(Header)*pow( 2, m-b-r-1 ));

	// Make sure r isn't last level (ie. basicBlockSize).
	if ( r < 0 || r >= MAX_RRANK ){
		printf("\nERROR IN SPLIT: r less than 0, or r greater than MAX_RRANK.\n\n");
		return NULL;					// r is out of range
	}
	// check if tier is empty
	if ( FL[r] == NULL ) {
		printf("\nERROR IN SPLIT: tier of rank %d, is empty. Cannot split block from empty list.\n\n", r);
		return NULL;						// can't move anything if tier is empty
	}

	Header* index_h = FL[r];		// index_h set to point to first r-header
	// printf("\nA_BEFORE_SPLIT_ADDR: %p", index_h);
	// printf("\nA_BEFORE_SPLIT_NEXT: %p", index_h->NEXT);
	// printf("\nA_BEFORE_SPLIT_SIZE: %d", index_h->SIZE);
	// printf("\nA_BEFORE_SPLIT_BUDDY: %c", index_h->BUDDY);
	// printf("\nA_BEFORE_SPLIT_MAGIC: %d\n", index_h->MAGIC);
	// remove index_h from r-list; **works when tier has one entry
	FL[r] = index_h->NEXT;
	// add index_h to (r+1)-list; adjust size to match new tier
	index_h->NEXT = FL[r+1];
	FL[r+1] = index_h;
	index_h->SIZE = pow( 2, m-r-1 );


	// make index_h into 'A' buddy
	index_h->BUDDY = 'A';			// becomes 'A' buddy in new list regardless of previous value
	// create buddy
	Header* buddy = (void*)((int8_t*)index_h + r_block_size);

	buddy->NEXT = NULL;					// initialize buddy values
	buddy->SIZE = index_h->SIZE;		//
	buddy->MAGIC = MAGIC;				//
	buddy->BUDDY = 'B';					// created buddy is always 'B'

	putAfter(index_h,buddy);			// add buddy to (r+1)list

	// printf("\nA_AFTER_SPLIT_ADDR: %p", index_h);
	// printf("\nA_AFTER_SPLIT_NEXT: %p", index_h->NEXT);
	// printf("\nA_AFTER_SPLIT_SIZE: %d", index_h->SIZE);
	// printf("\nA_AFTER_SPLIT_BUDDY: %c", index_h->BUDDY);
	// printf("\nA_AFTER_SPLIT_MAGIC: %d\n", index_h->MAGIC);

	// printf("\nB_AFTER_SPLIT_ADDR: %p", buddy);
	// printf("\nB_AFTER_SPLIT_NEXT: %p", buddy->NEXT);
	// printf("\nB_AFTER_SPLIT_SIZE: %d", buddy->SIZE);
	// printf("\nB_AFTER_SPLIT_BUDDY: %c", buddy->BUDDY);
	// printf("\nB_AFTER_SPLIT_MAGIC: %d\n", buddy->MAGIC);

	return index_h;
}

// takes pointer to one buddy
// returns address of joined buddies, NULL if unsuccessful
Header* join(Header* buddy){
	Header** FL = FL_MEM;
	int m = log2(MEM_SIZE);
	int b = log2(BLOCK_SIZE);

	// printf("\nBUDDY1_ADDR: %p", buddy);
	// printf("\nBUDDY1_NEXT: %p", buddy->NEXT);
	// printf("\nBUDDY1_SIZE: %d", buddy->SIZE);
	// printf("\nBUDDY1_BUDDY: %c", buddy->BUDDY);
	// printf("\nBUDDY1_MAGIC: %d\n", buddy->MAGIC);

	if( buddy == NULL ){
		printf("\nERROR IN JOIN(): JOIN() WAS PASSED A NULL* AS ARGUMENT.\n");
		return NULL;		// buddy should not be NULL
	}
	int rr = m - log2(buddy->SIZE);
	int r_block_size = pow(2, m-rr ) + sizeof(Header)*pow(2, m-b-rr );

	// get rr from size, and first_buddy from BUDDY
	if( FL[rr] == NULL ){
		printf("\nERROR IN JOIN(): LIST IS EMPTY.\n");
		return NULL;		// FL[rr] should not be empty
	}
	if( rr == 0 ){
		// printf("\nERROR IN JOIN(): RANK=0. CAN'T JOIN ELEMENT FROM ZEROTH RANK.\n"); // allowable
		return NULL;					// can't join zeroth tier or negative
	}
	if( rr < 0 ){
		printf("\nERROR IN JOIN(): INDEX BECAME NEGATIVE.\n");
		return NULL;					// can't join zeroth tier or negative
	}

	char first_buddy = buddy->BUDDY;
	// char second_buddy = ( first_buddy == 'A' ? 'B' : 'A' );

	// find the appropriate offset address for the given buddy
	// base+sizeof(Header) if 'A' buddy; base-sizeof(Header) if 'B' buddy
	Addr offset = NULL;
	if( first_buddy == 'A' ){
		offset = (void*)((int8_t*)buddy + r_block_size);
	}
	else if( first_buddy == 'B' ){
		offset = (void*)((int8_t*)buddy - r_block_size);
	}
	else{
		printf("\nERROR IN JOIN(): header did not have valid BUDDY value.\n");
		return NULL; 						// buddy labeling error
	}

	Header* tmp2 = FL[rr];			// initialized to first in rr-list
	Header* tmp1 = NULL;
	Header* match = NULL;
	_Bool found = 0;

	// FIND buddy's match:
	// check tmp2
	if( tmp2 == offset ){			// if match is first in free list
		found = 1;						// set found flag
		match = (Header*)tmp2;					// identify match
	}else{							// go on to check the second
		tmp1 = tmp2->NEXT;				// tmp1 will always be after tmp2
		if (tmp1 == NULL){
			//printf("\nERROR IN JOIN(): tmp1 = NULL (only one entry in free list).\n"); // ALLOWABLE
			return NULL;	// make sure a second entry exists
		}
	}

	// printf("\nOFFSET_ADDRESS: %p", offset);
	// printf("\ntmp1_ADDRESS: %p\n", tmp1);

	// check tmp1
	if( !found && tmp1 == offset ){	// if match is second in free list
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
	if( match == NULL ){
		printf("\nERROR IN JOIN(): match was not found.\n");
		return NULL;
	}
		// After this, match should be found (if it exists)
		// and tmp2 should be the header immediately before the match.
		// First buddy should always be first in the list.

	// remove both buddies from free list and add 'A' buddy to (r-1) list
	if( removeNext(tmp2) != 0 ){	// remove the match
		printf("\nERROR IN JOIN(): removeNext() unsuccessful.\n");
		return NULL;				//
	}
	FL[rr] = buddy->NEXT;		// remove first buddy

	// determine which buddy is which
	Header* A = ( first_buddy == 'A' ? buddy : match );
	Header* B = ( first_buddy == 'B' ? buddy : match );
	if( A->BUDDY == B->BUDDY ){
		printf("\nERROR IN JOIN(): SHOULD NOT HAPPEN.\n");
		return NULL;		// should not happen
	}

	// add A buddy to (rr-1) list
	Header* temp = FL[rr-1];
	FL[rr-1] = A;
	A->NEXT = temp;
	A->SIZE = (A->SIZE)*2;

	int AOrB = AorB( (Addr)A, rr-1 );
	if     ( AOrB == 0 )
		return NULL;					// unsuccessful
	else if( AOrB == 1 )
		A->BUDDY = 'A';
	else if( AOrB == 2 )
		A->BUDDY = 'B';
	// printf("\nMATCH_ADDR: %p", match);
	// printf("\nMATCH_NEXT: %p", match->NEXT);
	// printf("\nMATCH_SIZE: %d", match->SIZE);
	// printf("\nMATCH_BUDDY: %c", match->BUDDY);
	// printf("\nMATCH_MAGIC: %d\n", match->MAGIC);

	// printf("\nJOINED_ADDR: %p", A);
	// printf("\nJOINED_NEXT: %p", A->NEXT);
	// printf("\nJOINED_SIZE: %d", A->SIZE);
	// printf("\nJOINED_BUDDY: %c", A->BUDDY);
	// printf("\nJOINED_MAGIC: %d\n", A->MAGIC);
	return A;
}

// Determines whether a header is associated with an A or B buddy
// when given the memory tier, rr.
// Returns 1 for 'A,' or 2 for 'B,' and 0 if unsuccessful.
int AorB(Addr header, int rr){
	int m = log2(MEM_SIZE);
	int b = log2(BLOCK_SIZE);
	uintptr_t mem_start  = (uintptr_t)MEMORY;				// change memory ptr to integer
	uintptr_t head_start = (uintptr_t)((int8_t*)header);	// change header address to integer

	int r_block_size = pow(2, m-rr ) + sizeof(Header)*pow(2, m-b-rr );	// get block size

	int diff = head_start - mem_start;			// calculate space between two memory 
	diff = diff / r_block_size;						// divide by block size
	diff = diff % 2;								// determine odd/even

	if( diff == 1 ) return 2;		// if odd, 'B' buddy
	else if( diff == 0 ) return 1;	// if even, 'A' buddy
	else return 0;					// else, i don't even know
}

void list_lists(){
	Header** FL = FL_MEM;
	int counter = 0;

	Header* tmph = FL[0];
	printf("\n\n");
	for(int i = 0; i < MAX_RRANK; i++){
		counter = 0;
		tmph = FL[i];
		printf("[%2d]-", i);
		if(tmph != NULL){		// if list not empty
			printf("->[%15p %6x %1c %1d]--",tmph,tmph->SIZE,tmph->BUDDY,(tmph->MAGIC==MAGIC));
			counter++;
			if(counter > 3 ){
				printf("\n     ");
				counter = 0;
			}
		}
		else{					// if list is empty, go to next tier
			printf("\n");
			continue;
		}
		if(tmph->NEXT != NULL)	// if there is a second header
			tmph = tmph->NEXT;
		else{					// if not, go to next tier
			printf("\n");
			continue;
		}
		while(tmph->NEXT != NULL){		// while not on last header
			if( counter >= 4 ){
				printf("\n     ");
			}
			printf("->[%15p %6x %1c %1d]--",tmph,tmph->SIZE,tmph->BUDDY,(tmph->MAGIC==MAGIC));
			counter++;
			if(counter > 3 ){
				printf("\n     ");
				counter = 0;
			}
			tmph = tmph->NEXT;
		}
		printf("->[%15p %6x %1c %1d]--",tmph,tmph->SIZE,tmph->BUDDY,(tmph->MAGIC==MAGIC));
		counter++;
		if(counter > 3 ){
			printf("\n     ");
			counter = 0;
		}
		printf("\n");
	}
	return;
}