#include <iostream>
#include "compiler.h"

int main(const int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "Incorrect arguments\nShould use \"virgo <filename>.vi\"" << std::endl;
		return EXIT_FAILURE;
	}


	const Compiler compiler(argv[1]);
	compiler.compile();

	return EXIT_SUCCESS;
}