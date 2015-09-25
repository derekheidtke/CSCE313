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
    struct Header* NEXT;
    int size;
    int MAGIC;
  };
  typedef struct Header Header;

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length);

void release_allocator(void);

Addr my_malloc(unsigned int _length);

int my_free(Addr _a);

//========================================================
// HELPER FUNCTIONS


int removeNext(Header*);

int putAfter(Header*, Header*);

int moveDown(int, Header*);

int moveUp(int, Header*);


#endif 
