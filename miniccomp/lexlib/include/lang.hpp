//
// Created by Матвей Щербаков on 08.04.2024.
//

#ifndef MINICCOMP_LANG_H
#define MINICCOMP_LANG_H

#include <string>

enum TokenType {
    opinc, // 0
    opplus, // 1
    opeq, // 2
    opassign, // 3
    opne, // 4
    opnot, // 5
    oplt, // 6
    ople, // 7
    opor, // 8
    opand, // 9
    lpar, // ( 10
    rpar, // 11
    lbrace, // { 12
    rbrace, // 13
    semicolon, // 14
    comma, // 15
    period, // 16
    opgt, // 17
    opmul, // 18
    kchar, // 19
    kstr, // 20
    kid, // 21
    keyword, // 22
    opminus, // 23
    knum, // 24
    INVALID, // 25
    END_OF_FILE // 26
};

struct Token {
    TokenType type;
    std::string value;

    explicit Token(TokenType type)
        : type(type)
    {
    }

    Token(TokenType type, std::string value)
        : type(type)
        , value(std::move(value))
    {
    }
};

#endif // MINICCOMP_LANG_H
