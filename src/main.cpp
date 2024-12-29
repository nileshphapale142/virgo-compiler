#include <iostream>
#include "compiler.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "Incorrect arguments\nShould use \"virgo <filename>.vi\"" << std::endl;
		return 0;
	}

	Compiler compiler(argv[1]);
	compiler.compile();

	return 0;
}