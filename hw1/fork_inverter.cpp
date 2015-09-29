#include <iostream>
#include <string>
#include <cstdio>
#include "apue.h"


void string_invert(std::string);

int main(){

	pid_t 		pid;
	std::string in_str;
	std::string out_str;

	std::cerr << "\n>> String Inverter that uses fork()";
	std::cerr << "\n>> Please enter a string: ";

	std::getline(std::cin, in_str);

	std::cerr << "\n\tYOU ENTERED: " << in_str << std::endl;

	string_invert(in_str);

	return 0;
}

void string_invert(std::string IN){

	std::cerr << "\tREVERSED: ";
	
	std::string OUT   = std::string("");
	int 		SIZE  = IN.size();
	int 		index = SIZE-1;
	pid_t 		pid;
	int 		status;

	// fork
	for ( int j = 0; j < SIZE; j++) {	// create SIZE number of forks

		if ((pid = fork()) == 0) {		// child, break out
			break;
		} else {						// parent keep making children
			wait(NULL);
			index--;
		}
	}

	std::cerr << IN[index];
	if(index == 0) std::cout << std::endl << std::endl;
	_Exit(0);
}