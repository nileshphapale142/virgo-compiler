#pragma once

#include <string>
#include <vector>
#include "token.h"

class Scanner {
    public:
        explicit Scanner(std::string code);
        std::vector<Token> scan();
    private:
        std::optional<char> peek();
        std::optional<char> consume();

        std::string code;
        int curr_index;
};