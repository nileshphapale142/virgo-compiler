#pragma once

#include <string>

class Compiler {
public:
    Compiler(const std::string &filename);
    void compile();

private:
    std::string code;
    std::string filename;
};