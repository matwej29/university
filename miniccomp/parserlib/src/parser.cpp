//
// Created by Матвей Щербаков on 08.04.2024.
//
#include "parser.h"

#include <iostream>
#include <map>
#include <utility>

// FIXME: delete it, but may be not
#include "lexer.hpp"

Parser::Parser(std::function<Token()> getNextToken)
        : getNextToken(std::move(getNextToken)) {
    currentToken = this->getNextToken();
}

// FIXME: rename to setNextToken
void Parser::setCurrentToken() {
    this->currentToken = this->getNextToken();
}

bool Parser::validate() {
    std::cout.setf(std::ios::unitbuf);
    bool result = Parser::Expr();

    // graph
    std::vector<std::pair<std::string, int>> activeNonterms;
    bool wasLineBreak = false;
    for (const auto &x: callsHierarchy) {
        int current_depth = std::get<0>(x);
        bool current_entered = std::get<1>(x);
        std::string current_term = std::get<2>(x);
        std::string current_nonterm = std::get<3>(x);

        if (wasLineBreak) {
            for (auto i = 0; i < current_depth - current_entered; ++i) {
                bool found = false;
                for (const auto &a_nT: activeNonterms) {
                    // вроде можно за O(1)
                    if (a_nT.second == i) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    std::cout << "│ ";
                } else {
                    std::cout << "  ";
                }
            }

            if (current_entered) {
                std::cout << "├ ";
            }
        }

        if (!current_term.empty()) {
            std::cout << current_term << ' ';
            if (current_entered) {
                wasLineBreak = false;
                continue;
            }
        }
        if (current_entered and !current_nonterm.empty()) {
            activeNonterms.emplace_back(current_nonterm, current_depth);
            std::cout << current_nonterm << std::endl;
            wasLineBreak = true;
        } else {
            if (!current_nonterm.empty()) {
                // вроде нужно искать с конца
                auto ind = std::find(activeNonterms.begin(), activeNonterms.end(),
                                     std::pair<std::string, int>(current_nonterm, current_depth));
                if (ind != activeNonterms.end()) {
//                    if(*ind == activeNonterms.back()){
//                        std::cout << "really end ";
//                    } else {
//                        std::cout << "not end";
//                    }
                    activeNonterms.erase(ind);
                    std::cout << "└ ";
                    wasLineBreak = false;
                } else {
                    std::cout << current_nonterm << std::endl;
                    wasLineBreak = true;
                }
            } else {
                std::cout << std::endl;
                wasLineBreak = true;
            }
        }
    }

    std::cout << std::endl;
    if (currentToken.type != TokenType::END_OF_FILE) {
        return false;
    }
    return result;
}

//bool Parser::DeclareStmt() {
//    if (not Type()) {
//        return false;
//    }
//    if (currentToken.type == TokenType::kid) {
//        setCurrentToken();
//    } else {
//        return false;
//    }
//    if (not DeclareStmtL) {
//        return false;
//    }
//    return true;
//}
//
//bool Parser::DeclareStmtL() {
//    if (currentToken.type == TokenType::lpar) {
//        setCurrentToken();
//        if (not Param()) {
//            return false;
//        }
//        if (currentToken.type == TokenType::rpar) {
//            setCurrentToken();
//        } else {
//            return false;
//        }
//        if (currentToken.type == TokenType::lbrace) {
//            setCurrentToken();
//        } else {
//            return false;
//        }
//        if (not StmtList()) {
//            return false;
//        }
//        if (currentToken.type == TokenType::rbrace) {
//            setCurrentToken();
//        } else {
//            return false;
//        }
//        return true;
//    } else if (currentToken.type == TokenType::opassign) {
//        setCurrentToken();
//        if (currentToken.type == TokenType::knum) {
//            setCurrentToken();
//        } else {
//            return false;
//        }
//        if (not DeclareVarL()) {
//            return false;
//        }
//        if (currentToken.type == TokenType::semicolon) {
//            setCurrentToken();
//        } else {
//            return false;
//        }
//        return true;
//    } else if (DeclareVarL()) {
//        if (currentToken.type == TokenType::semicolon) {
//            return true;
//        } else {
//            return false;
//        }
//    }
//    return false;
//}
//
//bool Parser::Type() {
//    if (currentToken.type == TokenType::keyword and (currentToken.value == "int" or currentToken.value == "char")) {
//        setCurrentToken();
//        return true;
//    }
//    return false;
//}
//
//bool Parser::DeclVarList() {
//    if (currentToken.type == TokenType::comma) {
//        setCurrentToken();
//        if (currentToken.type == TokenType::kid) {
//            setCurrentToken();
//        } else {
//            return false;
//        }
//        if (not InitVar()) {
//            return false;
//        }
//        if (not DeclVarList()) {
//            return false;
//        }
//        return true;
//    }
//    return true;
//}
//
//bool Parser::InitVar() {
//    if (currentToken.type != TokenType::opassign) {
//        return false;
//    }
//    setCurrentToken();
//    if (currentToken.type == TokenType::knum or
//        currentToken.type == TokenType::kchar) {
//        setCurrentToken();
//        return true;
//    }
//    return false; // никаких UB
//}
//
//bool Parser::Param() {
//    if (Type()) {
//        if (currentToken.type == TokenType::kid) {
//            setCurrentToken();
//        } else {
//            return false;
//        }
//        if (not ParamList()) {
//            return false;
//        }
//    }
//
//    return true;
//}
//
//bool Parser::ParamList() {
//    if (currentToken.type == TokenType::comma) {
//        setCurrentToken();
//        if (not Type()) {
//            return false;
//        }
//        if (currentToken.type == TokenType::kid) {
//            setCurrentToken();
//        } else {
//            return false;
//        }
//        if (not ParamList()) {
//            return false;
//        }
//    }
//    return true;
//}
//
//bool Parser::StmtList() {
//    if (not Stmt()) {
//        return true;
//    }
//    if (not StmtList()) {
//        return false;
//    }
//    return true;
//}
//
//bool Parser::Stmt() {
//    if (DeclareStmt()) {
//        return true;
//    }
//    if (AssignOrCallOp()) {
//        return true;
//    }
//    if (WhileOp) {
//        return true;
//    }
//    if (ForOp) {
//        return true;
//    }
//    if (IfOp) {
//        return true;
//    }
//    if (SwitchOp) {
//        return true;
//    }
//    if (IOp) {
//        return true;
//    }
//    if (OOp) {
//        return true;
//    }
//    if (currentToken.type == TokenType::semicolon) {
//        setCurrentToken();
//        return true;
//    }
//    if (currentToken.type == TokenType::lbrace) {
//        setCurrentToken();
//        if (not StmtList()) {
//            return false;
//        }
//        if (currentToken.type == TokenType::rbrace) {
//            setCurrentToken();
//            return true;
//        } else {
//            return false;
//        }
//    }
//    if (currentToken.type == TokenType::keyword and currentToken.value == "return") {
//        setCurrentToken();
//        if (not Expr()) {
//            return false;
//        }
//        if (currentToken.type == TokenType::semicolon) {
//            setCurrentToken();
//            return true;
//        }
//        return false;
//    }
//    return false;
//}
//
//bool Parser::AssignOrCallOp(){
//    if(not AssignOrCall()){
//        return false;
//    }
//    if(currentToken.type == TokenType::semicolon){
//        setCurrentToken();
//        return true;
//    }
//    return false;
//}
//
//bool Parser::AssignOrCall(){
//    if(currentToken.type == TokenType::kid){
//        setCurrentToken();
//        if(not AssignOrCallL()){
//            return false;
//        }
//        return true;
//    }
//    return false;
//}
//
//bool Parser::AssignOrCallL(){
//
//}


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
