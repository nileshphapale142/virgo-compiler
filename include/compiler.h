#pragma once

#include <string>

class Compiler {
public:
    explicit Compiler(const std::string &filename);
    void compile() const;

private:
    std::string code;
    std::string filename;
};