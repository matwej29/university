//
// Created by Матвей Щербаков on 08.04.2024.
//
#ifndef MINICCOMP_PARSER_H
#define MINICCOMP_PARSER_H

#include <functional>
#include <fstream>
#include <map>
#include <string>
#include <expected>
#include <stack>


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

using Semantic = std::expected<std::string, std::string>;

class Parser {
public:
    Parser(std::function<Token()> getNextToken, std::string atoms_path)
            : getNextToken(getNextToken), atoms_path(atoms_path) {
        setCurrentToken();
    };

    bool validate();

    std::string atoms_path;
    std::ofstream stream;

    std::vector<std::tuple<int, bool, std::string, std::string>> callsHierarchy;
    int call_depth = 0;

    void printAtoms();

private:
    std::vector<Token> rollbackTokens;
    std::function<Token()> getNextToken;
    Token currentToken = {TokenType::INVALID, ""};

    int labelCounter = 0;
    int newVarCounter = 0;

    std::string newLabel();

    std::string alloc(const std::string &scope);

    std::expected<std::string, std::string>
    addVar(const std::string &name, const std::string &scope, const std::string &type, const std::string &init);

    std::expected<std::string, std::string>
    addFunc(const std::string &name, const std::string &type, const std::string &init);

    std::expected<std::string, std::string> checkVar(const std::string &name, const std::string &scope);

    std::expected<std::string, std::string> checkFunc(const std::string &name, const std::string &length);

    // context -> {name, scope, type, init, kind, length, cnt}
    std::map<std::string, std::vector<VarOrFunc>> varTable;

    void generateAtom(const std::string &context, const std::string &text, const std::string &first,
                      const std::string &second, const std::string &third);

    std::vector<Atom> atoms;
    std::stack<std::string> contextStack;

    void draw_calls_graph();

    void setCurrentToken();

    bool DeclareStmt();

    bool DeclareStmtL(const std::string &type, const std::string &name);

    Semantic Type();

    bool DeclVarList(const std::string &type);

    bool InitVar(const std::string &r, const std::string &s);

    Semantic Param();

    Semantic ParamList();

    bool StmtList();

    bool Stmt();

    bool AssignOrCallOp();

    bool AssignOrCall();

    bool AssignOrCallL(const std::string &name);

    bool WhileOp();

    bool ForOp();

    bool ForInit();

    Semantic ForExpr();

    bool ForLoop();

    bool IfOp();

    bool ElsePart();

    bool SwitchOp();

    bool Cases(const std::string &p, const std::string &end);

    bool CasesL(const std::string &p, const std::string &end, const std::string &def);

    Semantic ACase(const std::string &p, const std::string &end);

    bool InOp();

    bool OutOp();

    bool OutOpL();

    Semantic Expr();

    Semantic Expr7();

    Semantic Expr7List(const std::string &funcID);

    Semantic Expr6();

    Semantic Expr6List(const std::string &funcID);

    Semantic Expr5();

    Semantic Expr5List(const std::string &funcID);

    Semantic Expr4();

    Semantic Expr4List(const std::string &funcID);

    Semantic Expr3();

    Semantic Expr3List(const std::string &funcID);

    Semantic Expr2();

    Semantic Expr1();

    Semantic Expr1List(const std::string &funcID);

    Semantic Arg();

    Semantic ArgList();
};

#endif // MINICCOMP_PARSER_H
