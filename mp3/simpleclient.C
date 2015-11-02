/* 
	File: simpleclient.C

	Author: R. Bettati
			Department of Computer Science
			Texas A&M University
	Date  : 2013/01/31

	Simple client main program for MP3 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

	/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "reqchannel.H"
#include "BoundedBuffer.H"

using namespace std;

#define OPTSTR "n:b:w:"  // getopt() option list

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

BoundedBuffer* BUFFER;

void * stats_thread(void * arg);
void * req_thread(void * arg);
void * work_thread(void * arg);

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
	// PARSE INPUT ARGUMENTS
	unsigned int num_requests;
	unsigned int buf_size;
	unsigned int num_workers;
	int n_flag = 0,		// Used to provide default argument values if
		b_flag = 0,		// none are given.
		w_flag = 0;
	opterr = 0;			// Disable getopt() error message.

	// gets the arguments from the command line and rounds them up to the nearest
	// power of two
	int c;
	while( (c = getopt(argc, argv, OPTSTR)) != -1 ){
		switch(c){
			case 'n':
					 n_flag = 1;
					 num_requests 	= atoi(optarg);
					 break;
			case 'b':
					 b_flag = 1;
					 buf_size 		= atoi(optarg);
					 break;
			case 'w':
					 b_flag = 1;
					 num_workers 	= atoi(optarg);
					 break;
		};
	}
	if ( !n_flag ){
		num_requests = 1000; // Set default to 1000 requests
	}
	if ( !b_flag ){
		buf_size = 10; 	// Set default buffer size to 10 requests
	}
	if ( !w_flag ){
		num_workers = 3; // Set default to 3 workers
	}
	
	// create a bounded buffer with the specified size
	BoundedBuffer tmp = BoundedBuffer(buf_size);
	BUFFER = &tmp;

	cout << "CLIENT STARTED:" << endl;

	cout << "Establishing control channel... " << flush;
	RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
	cout << "done." << endl;

	/* -- Start sending a sequence of requests */

	string reply1 = chan.send_request("hello");
	cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;

	string reply2 = chan.send_request("data Joe Smith");
	cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << endl;

	string reply3 = chan.send_request("data Jane Smith");
	cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl;

	string reply5 = chan.send_request("newthread");
	cout << "Reply to request 'newthread' is " << reply5 << "'" << endl;
	RequestChannel chan2(reply5, RequestChannel::CLIENT_SIDE);

	string reply6 = chan2.send_request("data John Doe");
	cout << "Reply to request 'data John Doe' is '" << reply6 << "'" << endl;

	string reply7 = chan2.send_request("quit");
	cout << "Reply to request 'quit' is '" << reply7 << "'" << endl;

	string reply4 = chan.send_request("quit");
	cout << "Reply to request 'quit' is '" << reply4 << "'" << endl;

	usleep(1000000);
}

//====================================================================================

void * stats_thread(void * arg);
void * req_thread(void * arg);
void * work_thread(void * arg);
