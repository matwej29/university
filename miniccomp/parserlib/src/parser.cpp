//
// Created by Матвей Щербаков on 08.04.2024.
//
#include "parser.h"

#include <iostream>
#include <map>
#include <utility>

Parser::Parser(std::function<Token()> getNextToken)
        : getNextToken(std::move(getNextToken)) {
    currentToken = this->getNextToken();
}

void Parser::setCurrentToken() {
    this->currentToken = this->getNextToken();
}

bool Parser::validate() {
    bool result = Parser::Expr();

    // draw
    std::vector<std::pair<std::string, int>> ntActive;
    bool wasLB = false;
    for (auto x: callsHierarchy) {
        int c_depth = std::get<0>(x);
        bool c_active = std::get<1>(x);
        std::string c_term = std::get<2>(x);
        std::string c_nterm = std::get<3>(x);

        if (c_active) {
            ntActive.emplace_back(c_nterm, c_depth);
        }

        if (true) {
            for (auto i = 0; i < c_depth; ++i) {
                bool found = false;
                for (const auto &a_nT: ntActive) {
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
        }

        if (!c_term.empty()) {
            std::cout << c_term << " ";
        }
        if (c_active) {
            std::cout << c_nterm << std::endl;
            wasLB = true;
        } else {
            if (!c_nterm.empty()) {
                auto ind = std::find(ntActive.begin(), ntActive.end(), std::pair<std::string, int>(c_nterm, c_depth));
                if (ind != ntActive.end()) {
                    ntActive.erase(ind);
                    std::cout << "└" << std::endl;
                    wasLB = false;
                } else {
                    std::cout << c_nterm << std::endl;
                    wasLB = true;
                }
            } else {
                std::cout << std::endl;
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
        std::cout << "Expr7" << std::endl;
        callsHierarchy.pop_back();
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
        callsHierarchy.emplace_back(current_depth, true, "opor", "E7L");
        this->setCurrentToken();
        if (!Expr6()) {
            callsHierarchy.pop_back();
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
        callsHierarchy.pop_back();
        return false;
    }
    callsHierarchy.emplace_back(current_depth, false, "", "E6");
    bool result = this->Expr6List();
    call_depth = current_depth;
    return result;
}

bool Parser::Expr6List() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E6L");
    if (!Expr5()) {
        callsHierarchy.emplace_back(current_depth, false, "", "E6L");
        call_depth = current_depth;
        return true;
    }
    bool result = this->Expr7List();
    callsHierarchy.emplace_back(current_depth, false, "", "E6L");
    call_depth = current_depth;
    return result;
}

bool Parser::Expr5() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E5");
    if (!Expr4()) {
        callsHierarchy.pop_back();
        return false;
    }
    bool result = Expr5List();
    callsHierarchy.emplace_back(current_depth, false, "", "E5");
    call_depth = current_depth;
    return result;
}

bool Parser::Expr5List() {
    // TODO: refactor
    int current_depth = call_depth;
    call_depth += 1;
    if (this->currentToken.type == TokenType::opeq) {
        callsHierarchy.emplace_back(current_depth, true, "opeq", "E5L");
        this->setCurrentToken();
        if (!Expr4()) {
            callsHierarchy.pop_back();
            return false;
        }
    } else if (this->currentToken.type == TokenType::opne) {
        callsHierarchy.emplace_back(current_depth, true, "opne", "E5L");
        this->setCurrentToken();
        if (!Expr4()) {
            callsHierarchy.pop_back();
            return false;
        }
    } else if (this->currentToken.type == TokenType::opgt) {
        callsHierarchy.emplace_back(current_depth, true, "opgt", "E5L");
        this->setCurrentToken();
        if (!Expr4()) {
            callsHierarchy.pop_back();
            return false;
        }
    } else if (this->currentToken.type == TokenType::oplt) {
        callsHierarchy.emplace_back(current_depth, true, "oplt", "E5L");
        this->setCurrentToken();
        if (!Expr4()) {
            callsHierarchy.pop_back();
            return false;
        }
    } else if (this->currentToken.type == TokenType::ople) {
        callsHierarchy.emplace_back(current_depth, true, "ople", "E5L");
        this->setCurrentToken();
        if (!Expr4()) {
            callsHierarchy.pop_back();
            return false;
        }
    }
    callsHierarchy.emplace_back(current_depth, true, "", "E5L");
    callsHierarchy.emplace_back(current_depth, false, "", "E5L");
    return true;
}

bool Parser::Expr4() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E4");
    if (!Expr3()) {
        callsHierarchy.pop_back();
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
    if (this->currentToken.type == TokenType::opplus) {
        callsHierarchy.emplace_back(current_depth, true, "opplus", "E4L");
        this->setCurrentToken();
        if (not this->Expr3()) {
            callsHierarchy.pop_back();
            return false;
        }
//        callsHierarchy.emplace_back(current_depth, false, "", "E4L");
        call_depth = current_depth;
        bool result = Expr4List();
        return result;
    } else if (this->currentToken.type == TokenType::opminus) {
        callsHierarchy.emplace_back(current_depth, true, "opminus", "E4L");
        this->setCurrentToken();
        if (not this->Expr3()) {
            callsHierarchy.pop_back();
            return false;
        }
//        callsHierarchy.emplace_back(current_depth, false, "", "E4L");
        call_depth = current_depth;
        bool result = Expr4List();
        return result;
    }
    callsHierarchy.emplace_back(current_depth, true, "", "E4L");
    callsHierarchy.emplace_back(current_depth, false, "", "E4L");
    call_depth = current_depth;
    return true;
}

bool Parser::Expr3() {
    int current_depth = call_depth;
    call_depth += 1;
    callsHierarchy.emplace_back(current_depth, true, "", "E3");
    if (not Expr2()) {
        callsHierarchy.pop_back();
        return false;
    }
    callsHierarchy.emplace_back(current_depth, true, "", "E3");
    bool result = this->Expr3List();
    callsHierarchy.emplace_back(current_depth, false, "", "E3");
    call_depth = current_depth;
    return result;
}

bool Parser::Expr3List() {
    int current_depth = call_depth;
    call_depth += 1;
    if (this->currentToken.type == TokenType::opmul) {
        callsHierarchy.emplace_back(current_depth, true, "opmul", "E3L");
        this->setCurrentToken();
        if (not Expr2()) {
            callsHierarchy.pop_back();
            return false;
        }
//        callsHierarchy.emplace_back(current_depth, false, "", "E3L");
        call_depth = current_depth;
        bool result = this->Expr3List();
        return result;
    }
    callsHierarchy.emplace_back(current_depth, true, "", "E3L");
    callsHierarchy.emplace_back(current_depth, false, "", "E3L");
    call_depth = current_depth;
    return true;
}

bool Parser::Expr2() {
    int current_depth = call_depth;
    call_depth += 1;
    if (this->currentToken.type == TokenType::opnot) {
        callsHierarchy.emplace_back(current_depth, true, "opnot", "E2");
        setCurrentToken();
        if (not Expr1()) {
            callsHierarchy.pop_back();
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E2");
        call_depth = current_depth;
        return true;
    }
    callsHierarchy.emplace_back(current_depth, true, "", "E2");
    bool result = Expr1();
    if (!result){
        callsHierarchy.pop_back();
    } else {
        callsHierarchy.emplace_back(current_depth, false, "", "E2");
    }
    call_depth = current_depth;
    return result;
}

bool Parser::Expr1() {
    int current_depth = call_depth;
    call_depth += 1;
    if (this->currentToken.type == TokenType::opinc) {
        this->setCurrentToken();
        if (this->currentToken.type == TokenType::kid) {
            callsHierarchy.emplace_back(current_depth, true, "opinc kid", "E1");
            this->setCurrentToken();
            callsHierarchy.emplace_back(current_depth, false, "", "E1");
            call_depth = current_depth;
            return true;
        } else {
            callsHierarchy.pop_back();
            return false;
        }
    } else if (this->currentToken.type == TokenType::lpar) {
        callsHierarchy.emplace_back(current_depth, true, "lpar", "E1");
        this->setCurrentToken();
        if (!Expr()) {
            callsHierarchy.pop_back();
            return false;
        }
        if (this->currentToken.type == TokenType::rpar) {
            callsHierarchy.emplace_back(current_depth, true, "rpar", "E1");
            this->setCurrentToken();
        } else {
            callsHierarchy.pop_back();
            return false;
        }
        callsHierarchy.emplace_back(current_depth, false, "", "E1");
        call_depth = current_depth;
        return true;
    } else if (this->currentToken.type == TokenType::knum) {
        callsHierarchy.emplace_back(current_depth, true, "knum", "E1");
        callsHierarchy.emplace_back(current_depth + 1, false, this->currentToken.value, "");
        this->setCurrentToken();
        callsHierarchy.emplace_back(current_depth, false, "", "E1");
        call_depth = current_depth;
        return true;
    } else if (this->currentToken.type == TokenType::kid) {
        callsHierarchy.emplace_back(current_depth, true, "", "E1");
        callsHierarchy.emplace_back(current_depth + 1, false, this->currentToken.value, "");
        this->setCurrentToken();
        callsHierarchy.emplace_back(current_depth, false, "", "E1");
        return this->Expr1List();
    }

    return false;
}

bool Parser::Expr1List() {
    if (this->currentToken.type == TokenType::opinc) {
        callsHierarchy.emplace_back(call_depth, true, "opinc", "E1");
        this->setCurrentToken();
        callsHierarchy.emplace_back(call_depth, false, "", "E1");
        return true;
    }
    callsHierarchy.emplace_back(call_depth, true, "", "E1");
    callsHierarchy.emplace_back(call_depth, false, "", "E1");
    return true;
}
