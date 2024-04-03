/*
 * Created by Joshua Elkins
 * Date: June 24th, 2023
 */

#ifndef __PARSER__H__
#define __PARSER__H__


#include <iostream>
#include <vector>
#include <string>
#include <map>


#include "inputbuf.h"
#include "lexer.h"




class Parser {
public:
	LexicalAnalyzer lexer;
	Token token;
	TokenType tempTokenType;
	ScopeStack sStack;
	Statement curStatement;
	std::vector<Statement> statementList;
	std::vector<Token> symbolTable;
//	std::map<string, TokenType> mg;
	std::map<string, TokenType> mt;
	std::map<string, TokenType> mt2;
	
	int parse_program();
	int assign_types();
	int assign_types2();
	TokenType evaluateRHSType(int index);
	TokenType evaluateExpression(std::vector<Token>, std::vector<Token>);
	Token* buildTree(TokenTree**, Token*);
	TokenTree* assignTypesinTree(TokenTree* );
	void traverseTree(TokenTree*);
	TokenType evaluateNodes(TokenTree*, TokenTree*, TokenTree*);
	TokenType evaluateNewNodes(TokenTree*);
	string tokenTypeToString(TokenType);
	TokenType normalizeTokenTypes(TokenType);
	void loopback(Token);

	bool validate_c1();
	bool validate_c2();
	bool validate_c3();
	bool validate_c4();
	bool validate_c5();
	void print();

private:
	bool isBinaryMathOperator(TokenType);
	bool isBinaryLogicOperator(TokenType);
	bool isUnaryLogicOperator(TokenType);
	bool isUnaryLogicTokenType(TokenType);

	int parse_varlist();
	int parse_unaryOperator();
	int parse_binaryOperator();
	int parse_primary();
	int parse_expression();
	int parse_assstmt();
	int parse_case();
	int parse_caselist();
	int parse_switchstmt();
	int parse_whilestmt();
	int parse_ifstmt();
	int parse_stmt();
	int parse_stmtlist();
	int parse_body();
	int parse_typename();
	int parse_vardecl();
	int parse_vardecllist();
	int parse_globalVars();


};

#endif  //__PARSER__H__

