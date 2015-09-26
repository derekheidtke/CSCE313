/* 
    File: my_allocator.h

    Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

*/

#ifndef _my_allocator_h_                   // include file only once
#define _my_allocator_h_

#include <math.h>
#include "apue.h"
#include "ackerman.h"
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

  typedef void * Addr;

  struct Header
  {
    Addr NEXT;
    int SIZE;
    int MAGIC;
    char BUDDY;   // allowable values: 'A' or 'B'
  };
  typedef struct Header Header;

// returns amount of memory made available if successful, 0 if error
unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length);

void release_allocator(void);

// returns zero when out of memory or invalid arguments
Addr my_malloc(unsigned int _length);

// returns 0 from normal execution. non-zero if error occured.
int my_free(Addr _a);

//========================================================
// HELPER FUNCTIONS


int removeNext(Header*);

int putAfter(Header*, Header*);

// splits a block from freelist of rank r
int split(int);

// joins two headers in r-tier list
// move them to the next highest rank
// returns 0 if successful join
int join(int);


#endif 
