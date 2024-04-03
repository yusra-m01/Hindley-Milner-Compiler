/*
 * Copyright (C) Rida Bazzi, 2016
 * Edited by Joshua Elkins, 2023
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <istream>
#include <cstdio>
#include <stack>
using namespace std;

#include "inputbuf.h"

// ------- token types -------------------

typedef enum { END_OF_FILE = 0, INT, REAL, 
   BOO, TR, FA, IF, WHILE, SWITCH, CASE,
   PUBLIC, PRIVATE, NUM, REALNUM, NOT, PLUS,
   MINUS, MULT, DIV, GTEQ, GREATER, LTEQ,
   NOTEQUAL, LESS, LPAREN, RPAREN, EQUAL,
   COLON, COMMA, SEMICOLON, LBRACE,
   RBRACE, ID, ERROR, UNKNOWN
} TokenType;

typedef enum {
    ASSIGNMENT = 100, IFSTATEMENT, WHILESTATEMENT, SWITCHSTATEMENT, CASESTATEMENT, UNKNOWNSTATEMENT
}StatementTypes;


class Scope;
class Token {
  public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    TokenType value_type;
    int line_no;
    std::string scope;
    Scope* curScope;
};

class TokenTree {
public:
    TokenTree();
    Token t;
    TokenTree* left;
    TokenTree* right;
};

class Expression {
public:
    std::vector<Token> oList;
    std::vector<Token> vList;
    std::vector<Token> fList;
    TokenTree* pTree;
};

class Statement {
public:
    Statement();
    StatementTypes statementType;
    TokenType assignment;
    Token LHS;
    Expression RHS; 
    void initialize();
};

class LexicalAnalyzer {
  public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();
    void mapVariablesToScope(vector<Token>);
    string identifyScope(Token);
    bool searchIDinVector(vector<Token>, Token);
    std::vector<Token> tList;

  private:
    std::vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;

    bool SkipSpace();
    bool SkipComments();
    bool IsKeyword(std::string);
    TokenType FindKeywordIndex(std::string);
    Token ScanIdOrKeyword();
    Token ScanNumber();
};



class Scope {
public:
    Scope();
    Scope* parent;
    string curScope;
    vector<Token> publicVars;
    vector<Token> privateVars;
};

class ScopeStack {
public:
    ScopeStack();
    std::stack<Scope*> sStack;
};
#endif  //__LEXER__H__
