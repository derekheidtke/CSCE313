/* 
	File: my_allocator.c

	Author: <your name>
			Department of Computer Science
			Texas A&M University
	Date  : <date>

	Modified: 

	This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

	/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

	/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

	/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

	/* -- (none) -- */

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
     Coalescion?
  */
  free(_a);
  return 0;
}

unsigned int init_allocator(unsigned int _basic_block_size, 
		  unsigned int _length) {
	/* Use the malloc() function to request the correct amount of memory
	   from the runtime system.
	   Then call the 
	*/
	return 0;
}

int release_allocator() {
	/*
		This is called when the program exits normally.
	*/
	return 0;
}
