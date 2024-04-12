//
// Created by Матвей Щербаков on 08.04.2024.
//
#include <istream>
#include <functional>
#include "../../lex/include/lang.h"

#ifndef MINICCOMP_PARSER_H
#define MINICCOMP_PARSER_H

class Parser {
public:
    explicit Parser(std::function<Token()> getNextToken);
    bool validate();

    std::vector<std::tuple<int, std::string, std::string>> callsHierarchy;
    int call_depth = 0;

private:
//    std::istream &stream;
    std::vector<Token> rollbackTokens;
    std::function<Token()> getNextToken;
    Token currentToken = {TokenType::INVALID, ""};

    void setCurrentToken();

    bool Expr();
    bool Expr7();
    bool Expr7List();
    bool Expr6();
    bool Expr6List();
    bool Expr5();
    bool Expr5List();
    bool Expr4();
    bool Expr4List();
    bool Expr3();
    bool Expr3List();
    bool Expr2();
    bool Expr1();
    bool Expr1List();
};

#endif //MINICCOMP_PARSER_H
