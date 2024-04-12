//
// Created by Матвей Щербаков on 08.04.2024.
//
#include "parser.h"

// FIXME: remove this include
#include "lexer.h"

#include <utility>
#include <iostream>

Parser::Parser(std::function<Token()> getNextToken) : getNextToken(std::move(getNextToken)) {
    currentToken = this->getNextToken();
    std::cout << "Onstart: " << currentToken.type << std::endl;
}

void Parser::setCurrentToken() {
    this->currentToken = this->getNextToken();
    std::cout << TokenTypeToString.at(currentToken.type) << std::endl;
}

bool Parser::validate() {
    bool result = Parser::Expr();

    std::cout << "Onend: " << this->currentToken.type << std::endl;
    return result;
}

bool Parser::Expr() {
    int current_depth = call_depth;
    callsHierarchy.emplace_back(current_depth, "E", "");

    return this->Expr7();
}

bool Parser::Expr7() {
    if (not Parser::Expr6()) {
        std::cout << "Expr7" << std::endl;
        return false;
    }
    return this->Expr7List();
}

bool Parser::Expr7List() {
    if (this->currentToken.type == TokenType::opor) {
        this->setCurrentToken();
        if (not this->Expr6()) {
            std::cout << "Expr7List" << std::endl;
            return false;
        }

        return this->Expr7List();
    }

    return true;
}

bool Parser::Expr6() {
    if (not this->Expr5()) {
        std::cout << "Expr6" << std::endl;
        return false;
    }
    return this->Expr6List();
}

bool Parser::Expr6List() {
    if (not this->Expr5()) {
        return true;
    }
    return this->Expr6List();
}

bool Parser::Expr5() {
    if (not this->Expr4()) {
        std::cout << "Expr5" << std::endl;
        return false;
    }
    return this->Expr5List();
}

bool Parser::Expr5List() {
    // TODO: refactor
    if (this->currentToken.type == TokenType::opeq) {
        this->setCurrentToken();
        if (not this->Expr4()) {
            std::cout << "Expr5List" << std::endl;
            return false;
        }
    } else if (this->currentToken.type == TokenType::opne) {
        this->setCurrentToken();
        if (not this->Expr4()) {
            std::cout << "Expr5List" << std::endl;
            return false;
        }
    } else if (this->currentToken.type == TokenType::opgt) {
        this->setCurrentToken();
        if (not this->Expr4()) {
            std::cout << "Expr5List" << std::endl;
            return false;
        }
    } else if (this->currentToken.type == TokenType::oplt) {
        this->setCurrentToken();
        if (not this->Expr4()) {
            std::cout << "Expr5List" << std::endl;
            return false;
        }
    } else if (this->currentToken.type == TokenType::opeq) {
        this->setCurrentToken();
        if (not this->Expr4()) {
            std::cout << "Expr5List" << std::endl;
            return false;
        }
    }
    return true;
}

bool Parser::Expr4() {
    if (not this->Expr3()) {
        std::cout << "Expr4" << std::endl;
        return false;
    }
    return this->Expr4List();
}

bool Parser::Expr4List() {
    if (this->currentToken.type == TokenType::opplus) {
        this->setCurrentToken();
        if (not this->Expr3()) {
            std::cout << "Expr4List" << std::endl;
            return false;
        }
        return this->Expr4List();
    } else if (this->currentToken.type == TokenType::opminus) {
        this->setCurrentToken();
        if (not this->Expr3()) {
            std::cout << "Expr4List" << std::endl;
            return false;
        }
        return this->Expr4List();
    }
    return true;
}

bool Parser::Expr3() {
    if (not this->Expr2()) {
        std::cout << "Expr4List" << std::endl;
        return false;
    }
    return this->Expr3List();
}

bool Parser::Expr3List() {
    if (this->currentToken.type == TokenType::opmul) {
        this->setCurrentToken();
        if (not this->Expr2()) {
            std::cout << "Expr3List" << std::endl;
            return false;
        }
        return this->Expr3List();
    }
    return true;
}

bool Parser::Expr2() {
    if(this->currentToken.type == TokenType::opnot){
        setCurrentToken();
        if (not this->Expr1()){
            std::cout << "Expr2" << std::endl;
            return false;
        }
        return true;
    }
    return this->Expr1();
}

bool Parser::Expr1() {
    if (this->currentToken.type == TokenType::opinc){
        this->setCurrentToken();
        if (this->currentToken.type == TokenType::kid){
            this->setCurrentToken();
            return true;
        } else {
            return false;
        }
    } else if (this->currentToken.type == TokenType::lpar){
        this->setCurrentToken();
        if (not this->Expr()){
            std::cout << "Expr1" << std::endl;
            return false;
        }
        if (this->currentToken.type == TokenType::rpar){
            this->setCurrentToken();
        } else {
            return false;
        }
        return true;
    } else if (this->currentToken.type == TokenType::knum) {
        this->setCurrentToken();
        return true;
    } else if (this->currentToken.type == TokenType::kid){
        this->setCurrentToken();
        return this->Expr1List();
    }
    std::cout << "Expr1" << std::endl;
    return false;
}

bool Parser::Expr1List() {
    if (this->currentToken.type == TokenType::opinc){
        this->setCurrentToken();
        return true;
    }
    return true;
}



