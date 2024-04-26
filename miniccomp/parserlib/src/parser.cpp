//
// Created by Матвей Щербаков on 08.04.2024.
//
#include "parser.h"

#include <iostream>
#include <map>
#include <utility>

// FIXME: delete this
#include "lexer.hpp"

Parser::Parser(std::function<Token()> getNextToken)
        : getNextToken(std::move(getNextToken)) {
    currentToken = this->getNextToken();
}

void Parser::setCurrentToken() {
    this->currentToken = this->getNextToken();
}

bool Parser::validate() {
    std::cout.setf(std::ios::unitbuf);
    bool result = Parser::Expr();

    // draw
    std::vector<std::pair<std::string, int>> ntActive;
    bool wasLB = false;
    for (auto &x: callsHierarchy) {
        int c_depth = std::get<0>(x);
        bool c_active = std::get<1>(x);
        std::string c_term = std::get<2>(x);
        std::string c_nterm = std::get<3>(x);

        if (wasLB) {
            for (auto i = 0; i < c_depth - c_active; ++i) {
                bool found = false;
                for (const auto &a_nT: ntActive) {
                    // вроде можно за O(1)
                    if (a_nT.second == i) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    std::cout << "│ ";
                } else {
                    std::cout << " ";
                }
            }

            if (c_active and wasLB) {
                std::cout << "├";
            }
        }

        if (!c_term.empty()) {
            std::cout << ' ' << c_term << ' ';
            if (c_active) {
                wasLB = false;
                continue;
            }
        }
        if (c_active and !c_nterm.empty()) {
            ntActive.emplace_back(c_nterm, c_depth);
            std::cout << c_nterm << std::endl;
            wasLB = true;
        } else {
            if (!c_nterm.empty()) {
                // вроде нужно искать с конца
                auto ind = std::find(ntActive.begin(), ntActive.end(),
                                     std::pair<std::string, int>(c_nterm, c_depth));
                if (ind != ntActive.end()) {
                    ntActive.erase(ind);
                    std::cout << "└";
                    wasLB = false;
                } else {
                    std::cout << c_nterm << std::endl;
                    wasLB = true;
                }
            } else {
                std::cout << std::endl;
                wasLB = true;
            }
        }
    }

    std::cout << std::endl;
    if (currentToken.type != TokenType::END_OF_FILE) {
        return false;
    }
    return result;
}

bool Parser::Expr() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E");
    callsHierarchy.emplace_back(current_depth, false, "", "E");
    bool result = Expr7();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr7() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E7");
    if (!Expr6()) {
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E7");
    bool result = this->Expr7List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr7List() {
    int current_depth = call_depth;
    call_depth += 1;
    if (this->currentToken.type == TokenType::opor) {
        callsHierarchy.emplace_back(current_depth, true, "", "E7L");
        callsHierarchy.emplace_back(current_depth + 1, true, "opor", "");
        this->setCurrentToken();
        if (!Expr6()) {
            call_depth = current_depth;
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E7L");
        bool result = this->Expr7List();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E7L");
    call_depth = current_depth;
    return true;
}

bool Parser::Expr6() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E6");
    if (!Expr5()) {
        call_depth = current_depth;
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E6");
    bool result = Expr6List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr6List() {
    int current_depth = call_depth;
    call_depth += 1;
    if (this->currentToken.type == TokenType::opand) {
        callsHierarchy.emplace_back(current_depth, true, "", "E6L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        this->setCurrentToken();
        if (!Expr5()) {
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E6L");
        bool result = this->Expr6List();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E6L");
    call_depth = current_depth;
    return true;
}

bool Parser::Expr5() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E5");
    if (!Expr4()) {
        call_depth = current_depth;
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E5");
    bool result = Expr5List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr5List() {
    // TODO: refactor
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opeq) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr4()) {
            call_depth = current_depth;
            return false;
        }
    } else if (currentToken.type == TokenType::opne) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr4()) {
            call_depth = current_depth;
            return false;
        }
    } else if (currentToken.type == TokenType::opgt) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr4()) {
            call_depth = current_depth;
            return false;
        }
    } else if (currentToken.type == TokenType::oplt) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr4()) {
            call_depth = current_depth;
            return false;
        }
    } else if (currentToken.type == TokenType::ople) {
        callsHierarchy.emplace_back(current_depth, true, "", "E5L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (!Expr4()) {
            call_depth = current_depth;
            return false;
        }
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E5L");
    return true;
}

bool Parser::Expr4() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E4");
    if (!Expr3()) {
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E4");
    bool result = Expr4List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr4List() {
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opplus) {
        callsHierarchy.emplace_back(current_depth, true, "", "E4L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr3()) {
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E4L");
        bool result = Expr4List();
        call_depth = current_depth;
        return result;
    } else if (currentToken.type == TokenType::opminus) {
        callsHierarchy.emplace_back(current_depth, true, "", "E4L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr3()) {
            call_depth = current_depth;
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E4L");
        bool result = Expr4List();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E4L");
    call_depth = current_depth;
    return true;
}

bool Parser::Expr3() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E3");
    if (not Expr2()) {
        call_depth = current_depth;
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E3");
    bool result = Expr3List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr3List() {
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opmul) {
        callsHierarchy.emplace_back(current_depth, true, "", "E3L");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr2()) {
            call_depth = current_depth;
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E3L");
        bool result = Expr3List();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E3L");
    call_depth = current_depth;
    return true;
}

bool Parser::Expr2() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E2");
    if (currentToken.type == TokenType::opnot) {
        callsHierarchy.emplace_back(current_depth, false, "", "E2");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr1()) {
            call_depth = current_depth;
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E2");
    bool result = Expr1();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr1() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E1");
    if (currentToken.type == TokenType::opinc) {
        setCurrentToken();
        if (currentToken.type == TokenType::kid) {
            callsHierarchy.emplace_back(current_depth, false, "", "E1");
            callsHierarchy.emplace_back(current_depth, false, "opinc kid", "");
            setCurrentToken();
            call_depth = current_depth;
            return true;
        } else {
            call_depth = current_depth;
            return false;
        }
    } else if (this->currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth + 1, true, "lpar", "");
        setCurrentToken();
        if (!Expr()) {
            call_depth = current_depth;
            return false;
        }
        if (this->currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth, false, "", "E1");
            callsHierarchy.emplace_back(current_depth, false, "rpar", "");
            this->setCurrentToken();
            call_depth = current_depth;
            return true;
        } else {
            call_depth = current_depth;
            return false;
        }
    } else if (currentToken.type == TokenType::knum) {
        callsHierarchy.emplace_back(current_depth, false, "", "E1");
        callsHierarchy.emplace_back(current_depth, false, currentToken.value, "");
        setCurrentToken();
        call_depth = current_depth;
        return true;
    } else if (currentToken.type == TokenType::kid) {
        callsHierarchy.emplace_back(current_depth, false, "", "E1");
        callsHierarchy.emplace_back(current_depth, true, currentToken.value, "");
        setCurrentToken();
        bool result = Expr1List();
        call_depth = current_depth;
        return result;
    }

    call_depth = current_depth;
    return false;
}

bool Parser::Expr1List() {
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::opinc) {
        callsHierarchy.emplace_back(current_depth, true, "", "E1L");
        callsHierarchy.emplace_back(current_depth, false, "", "E1L");
        callsHierarchy.emplace_back(call_depth, false, "opinc", "");
        setCurrentToken();
    } else if (this->currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth, true, "", "E1L");
        callsHierarchy.emplace_back(current_depth + 1, true, "lpar", "");
        setCurrentToken();
        if (!Arg()) {
            call_depth = current_depth;
            return false;
        }
        if (this->currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth, false, "", "E1L");
            callsHierarchy.emplace_back(current_depth, false, "rpar", "");
            setCurrentToken();
        } else {
            call_depth = current_depth;
            return false;
        }
        call_depth = current_depth;
        return true;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E1L");
    call_depth = current_depth;
    return true;
}

bool Parser::Arg() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "Arg");
    if (not Expr()) {
        callsHierarchy.pop_back();
        callsHierarchy.emplace_back(current_depth, false, "", "Arg");
        return true;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "Arg");
    bool result = ArgList();
    call_depth = current_depth;
    return result;
}

bool Parser::ArgList() {
    int current_depth = call_depth;
    call_depth += 1;
    if (currentToken.type == TokenType::comma) {
        callsHierarchy.emplace_back(current_depth, true, "", "ArgList");
        callsHierarchy.emplace_back(current_depth + 1, true, TokenTypeToString.at(currentToken.type), "");
        setCurrentToken();
        if (not Expr()) {
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "ArgList");
        bool result = ArgList();
        call_depth = current_depth;
        return result;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "ArgList");
    return true;
}