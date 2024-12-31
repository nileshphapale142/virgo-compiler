#pragma once

#include <string>
#include <vector>
#include "token.h"

class Scanner {
    public:
        Scanner();
        std::vector<Token> scan(const std::string &code);
};