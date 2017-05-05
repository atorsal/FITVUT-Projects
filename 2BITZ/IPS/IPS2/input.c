#include "input.h"

std::vector<char *> read_stdin() {
	std::vector<char *> result;
	std::string line;
	while (std::getline(std::cin, line)) {
		char *str;
		str=(char *) malloc(sizeof(char)*(line.length()+1));
		strcpy(str,line.c_str());
		result.push_back(str);
	}	
	return result;
}
