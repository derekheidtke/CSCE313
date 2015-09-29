#include <iostream>
#include <string>
#include <cstdio>
#include "apue.h"


void string_invert(std::string);

int main(){

	pid_t pid;

	// if ((pid = fork()) < 0 ) {
	// 	std::cout << "\nfork error";
	// } else if ( pid == 0 ) {
	// 	std::cout << "\nCHILD: " << pid;
	// } else{
	// 	std::cout << "\nPARENT: 0";
	// }


	std::string in_str;
	std::string out_str;

	std::cout << "\n>> String Inverter that uses fork()";
	std::cout << "\n>> Please enter a string: ";

	std::getline(std::cin, in_str);

	std::cout << "\nYOU ENTERED: " << in_str;
	std::cout << "\n   REVERSED: ";
	fflush(NULL);

	string_invert(in_str);

	std::cout;// << std::endl;
	return 0;
}

void string_invert(std::string forward_str){
	int 	index = -1;
	pid_t 	pid;
	int status;

	//std::cout << forward_str[0] << std::endl;

	if( (pid = fork()) == 0 ){	// child
		while( index < forward_str.size() ){
			index++;
			if ( (pid = fork()) == 0 ) {}
			else {
				wait(&status);
				std::cout << forward_str[index];
				break;
			}
		}
	} else {					// parent
		wait(&status);
		std::cout << std::endl;
	}
}