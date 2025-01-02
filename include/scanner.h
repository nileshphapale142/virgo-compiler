#pragma once

#include <string>
#include <vector>
#include "token.h"

class Scanner {
    public:
        Scanner(const std::string &code);
        std::vector<Token> scan();
    private:
        std::optional<char> peek();
        std::optional<char> consume();
        
        int curr_index;
        std::string code; 
};