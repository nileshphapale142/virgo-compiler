#include <iostream>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Incorrect arguments\nShould use \"virgo <filename>.vi\"" << std::endl;
		return 0;
		
	}
	std::cout << "Filename is " << argv[1] << std::endl;
	std::cout << "What the fuck are doing World?" << std::endl;
	return 0;
}
