#pragma once

#include <string>
#include "parser.h"	
#include <sstream>

class CodeGenerator {
	public:
		CodeGenerator(P_Rule root);
		std::string generate();
	private:
		P_Rule root;
		std::stringstream output_code;
};