#include<iostream>
#include<vector>
#include<regex>
#include "input.h"



int main(int argc, char* argv[]){
	std::vector<char *> input=read_stdin();
	std::regex re("(.*)ahoj(.*)");
	std::regex re1(argv[1]);
	for(int i=0;i<input.size();i++) {
		std::cout << "XX: "<< input[i]<<"\n";
		// regex_match String musi odpovidat celemu reg. vyrazu
		if(std::regex_match(input[i],re))
			std::cout << "11: " <<input[i]<<"\n";
		//std::cout << std::regex_replace(input[i], re, "$1-joha-$2")<<"\n";
		std::cout << std::regex_replace(input[i], re1, argv[2])<<"\n";
	}

	return 0;
}
