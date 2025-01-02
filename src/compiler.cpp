#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "compiler.h"
#include "scanner.h"
#include "parser.h"
#include "codegen.h"

Compiler::Compiler(const std::string &filename) {

    if (!filename.ends_with(".vi")) {
        std::cout << "Unknown file type" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "No such " << filename << " file found" << std::endl;
        exit(EXIT_FAILURE);
    }

    this->filename = std::filesystem::path(filename).stem().string();

    std::stringstream buffer;
    buffer << file.rdbuf();
    this->code = buffer.str();

    return;
}

void Compiler::compile() {
    Scanner scanner;

    std::vector<Token> tokens = scanner.scan(this->code);
    
    // for (auto tok :tokens) std::cout << tok.tokenTypeToString() << " ";

    // std::cout << std::endl;

    Parser parser(tokens);

    auto root = parser.parse();

    CodeGenerator generator(root);

    std::string assembly_code = generator.generate();

    std::ofstream file("test/" + filename + ".asm");

    if (!file.is_open()) {
        std::cerr << "Unable to create file" << std::endl;
        exit(EXIT_FAILURE);
    }

    file << assembly_code;

    file.close();


    std::string nasm_cmd = "nasm -f elf64 -o ./test/" + filename + ".o ./test/" + filename + ".asm";
    std::string ld_cmd = "ld -o " + filename + " ./test/" + filename + ".o";
    
    system(nasm_cmd.c_str());
    system(ld_cmd.c_str());

    return;
}