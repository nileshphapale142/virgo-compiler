#pragma once

#include <string>
#include <vector>
#include "token.h"

class Scanner {
    public:
        explicit Scanner(std::string code);
        std::vector<Token> scan();
    private:
        std::optional<char> peek(int offset = 0);
        std::optional<char> consume();

        std::string code;
        int curr_index;
};