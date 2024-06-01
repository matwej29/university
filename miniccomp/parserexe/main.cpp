//
// Created by Матвей Щербаков on 08.04.2024.
//
#include <fstream>
#include <iostream>

#include "lexer.hpp"
#include "parser.h"

int main() {
    std::ifstream file("expr.txt");

    if (!file.is_open()) {
        std::cout << "File not found" << std::endl;
        return 1;
    }

    Lexer lex(file);
    auto a = [&lex]() { return lex.getNextToken(); };
    Parser parser(a, "output.txt");
    std::cout << parser.validate() << std::endl;
    parser.printAtoms();
}
