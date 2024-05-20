//
// Created by Матвей Щербаков on 08.04.2024.
//
#ifndef MINICCOMP_PARSER_H
#define MINICCOMP_PARSER_H

#include <functional>
#include <istream>
#include <map>
#include <string>
#include <expected>


#include "lang.hpp"

struct Atom {
    std::string context;
    std::string text;
    std::string first;
    std::string second;
    std::string third;
};

struct VarOrFunc {
    std::string name;
    std::string scope;
    std::string type;
    std::string init;
    std::string kind;
    std::string length;
    int cnt;
};

class Parser {
public:
    explicit Parser(std::function<Token()> getNextToken);
    bool validate();

    std::vector<std::tuple<int, bool, std::string, std::string>> callsHierarchy;
    int call_depth = 0;

private:
    //    std::istream &stream;
    std::vector<Token> rollbackTokens;
    std::function<Token()> getNextToken;
    Token currentToken = { TokenType::INVALID, "" };


    int labelCounter = 0;
    int newVarCounter = 0;

    std::string newLabel();
    std::string alloc(const std::string &scope);
    std::expected<std::string, std::string> addVar(const std::string &name, const std::string &scope, const std::string &type, const std::string &init);
    std::expected<std::string, std::string> addFunc(const std::string &name, const std::string &type, const std::string &init);
    std::expected<std::string, std::string> checkVar(const std::string &name, const std::string &scope);
    std::expected<std::string, std::string> checkFunc(const std::string &name, const int length);
    // context -> {name, scope, type, init, kind, length, cnt}
    std::map<std::string, std::vector<VarOrFunc>> varTable;


    void setCurrentToken();

    bool DeclareStmt();
    bool DeclareStmtL();
    bool Type();
    bool DeclVarList();
    bool InitVar();
    bool Param();
    bool ParamList();
    bool StmtList();
    bool Stmt();
    bool AssignOrCallOp();
    bool AssignOrCall();
    bool AssignOrCallL();
    bool WhileOp();
    bool ForOp();
    bool ForInit();
    bool ForExpr();
    bool ForLoop();
    bool IfOp();
    bool ElsePart();
    bool SwitchOp();
    bool Cases();
    bool CasesL();
    bool ACase();
    bool InOp();
    bool OutOp();
    bool OutOpL();

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

    bool Arg();
    bool ArgList();
};

#endif // MINICCOMP_PARSER_H
