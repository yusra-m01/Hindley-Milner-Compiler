/*
 * Created by: Joshua Elkins
 * Date: June 24th, 2023
 */

#include <iostream>
#include <string>
#include <list>
#include "parser.h"
#include "lexer.h"
#include "inputbuf.h"

using namespace std;

// Syntax Error Function.
void syntax_error() {
	cout << "Syntax Error\n";
	exit(1);
}

/*
 * Completed Function.
 * Entry point to the program.
 */
int Parser::parse_program() {
#ifdef DEBUG
	cout << "Entered Parse Program" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type == ID) {
		lexer.UngetToken(token);
		Scope* s = new Scope();
		s->curScope = "Global";
		s->parent = NULL;
		this->sStack.sStack.push(s);
		parse_globalVars();

		for (int i = 0; i < (int)lexer.tList.size(); i++)
		{
			if (!(mt2.find(lexer.tList[i].lexeme) != mt2.end()))
			{
				mt2[lexer.tList[i].lexeme] = lexer.tList[i].value_type;
			}
			else {// global variable already exists
				syntax_error();
			}
		}
		parse_body();
	}
	else if (token.token_type == LBRACE) {
		lexer.UngetToken(token);
		parse_body();
	}
	else {
		syntax_error();
	}

	return 0;
}

/*
 * Completed Function.
 * Acts as basic entry into the global variable list.
 */
int Parser::parse_globalVars() {
#ifdef DEBUG
	cout << "Entered Parse Global Variables" << endl;
#endif
	parse_vardecllist();

	return 0;
}

/*
 * Completed
 * Loops our variable declaration list.
 */
int Parser::parse_vardecllist() {
#ifdef DEBUG
	cout << "Entered Parse Variable Declaration List" << endl;
#endif
	token = lexer.GetToken();
	while (token.token_type == ID) {
		lexer.UngetToken(token);
		parse_vardecl();
		token = lexer.GetToken();
	}
	lexer.UngetToken(token);

	return 0;
}

/*
 * Completed Function
 * Acts as a method to handle the declaration statements.
 */
int Parser::parse_vardecl() {
#ifdef DEBUG
	cout << "Entered Parse Variable Declaration" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type != ID) {
		syntax_error();
	}
	lexer.UngetToken(token);
	parse_varlist();
	token = lexer.GetToken();
	if (token.token_type != COLON) {
		syntax_error();
	}
	token = lexer.GetToken();
	if (token.token_type == INT || token.token_type == REAL || token.token_type == BOO) {
		lexer.UngetToken(token);
		parse_typename();
		token = lexer.GetToken();
		if (token.token_type != SEMICOLON) {
			syntax_error();
		}
	}
	else {
		syntax_error();
	}

	return 0;
}


/*
 * Completed Function
 * Acts as the gathering function for our variables
 */
int Parser::parse_varlist() {
#ifdef DEBUG
	cout << "Entered Parse Variable List" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type != ID) {
		syntax_error();
	}
	else {
		Token t2 = lexer.GetToken();
		if (t2.token_type == COMMA) {
			while (token.token_type == ID && t2.token_type == COMMA) {
				// Gather ID token info here
			
				token.value_type = UNKNOWN; //for now, I will mark the type as unknown. 
				lexer.tList.push_back(token);
				token = lexer.GetToken();

				if (token.token_type != ID) {
					syntax_error();
				}
				t2 = lexer.GetToken();
				if (t2.token_type == COLON)
				{// capture the last variable here
					token.value_type = UNKNOWN;
					lexer.tList.push_back(token);
				}
			}
			lexer.UngetToken(t2);
		}
		else if (t2.token_type == COLON)
		{
			
			token.value_type = UNKNOWN; //for now, I will mark the type as unknown. 
			lexer.tList.push_back(token);
			lexer.UngetToken(t2);
		}
		else {
			// Gather singular ID token info here
			lexer.UngetToken(t2);
		}
	}

	return 0;
}

/*
 * Completed Function
 * Just consumes the INT, REAL, or BOO tokens
 */
int Parser::parse_typename() {
#ifdef DEBUG
	cout << "Entered Parse Type Name" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type == INT) {
		// We'll be gathering info here
		for (int i = 0; i < (int)lexer.tList.size(); i++)
		{
			if (lexer.tList[i].value_type == UNKNOWN)
				lexer.tList[i].value_type = INT;
			lexer.tList[i].curScope = this->sStack.sStack.top();
		}
	}
	else if (token.token_type == REAL) {
		for (int i = 0; i < (int)lexer.tList.size(); i++)
		{
			if (lexer.tList[i].value_type == UNKNOWN)
				lexer.tList[i].value_type = REAL;
			lexer.tList[i].curScope = this->sStack.sStack.top();
			
		}

	}
	else if (token.token_type == BOO) {
		for (int i = 0; i < (int)lexer.tList.size(); i++) 
		{
			if (lexer.tList[i].value_type == UNKNOWN)
				lexer.tList[i].value_type = BOO;
			lexer.tList[i].curScope = this->sStack.sStack.top();
		}

	}
	else {
		syntax_error();
	}

	return 0;
}

/*
 * Completed Function
 * Acts as the method to consume braces and enter statement list
 */
int Parser::parse_body() {
#ifdef DEBUG
	cout << "Entered Parse Body" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type == LBRACE) {
		parse_stmtlist();
		token = lexer.GetToken();
		if (token.token_type != RBRACE) {
			syntax_error();
		}
	}
	else {
		syntax_error();
	}

	return 0;
}

/*
 * Completed Function
 * Acts as our looper to enter all our statements
 */
int Parser::parse_stmtlist() {
#ifdef DEBUG
	cout << "Entered Parse Statement List" << endl;
#endif
	token = lexer.GetToken();
	while (token.token_type == ID || token.token_type == IF || token.token_type == WHILE || token.token_type == SWITCH) {
		lexer.UngetToken(token);
		parse_stmt();
		
		token = lexer.GetToken();
	}
	lexer.UngetToken(token);

	return 0;
}

/*
 * Completed Function
 * Acts as our method to enter the specific statements
 */
int Parser::parse_stmt() {
#ifdef DEBUG
	cout << "Entered Parse Statement" << endl;
#endif
	token = lexer.GetToken();
	this->curStatement.initialize();
	if (token.token_type == ID) {
		lexer.UngetToken(token);
		this->curStatement.statementType = ASSIGNMENT;
		parse_assstmt();
	}
	else if (token.token_type == IF) {
		lexer.UngetToken(token);
		this->curStatement.statementType = IFSTATEMENT;
		parse_ifstmt();
	}
	else if (token.token_type == WHILE) {
		lexer.UngetToken(token);
		this->curStatement.statementType = WHILESTATEMENT;
		parse_whilestmt();
	}
	else if (token.token_type == SWITCH) {
		lexer.UngetToken(token);
		this->curStatement.statementType = SWITCHSTATEMENT;
		parse_switchstmt();
	}
	else {
		syntax_error();
	}
	//this->statementList.push_back(this->curStatement);
	//this->curStatement.initialize();

	return 0;
}

Token* Parser::buildTree(TokenTree** p, Token* t)
{
	
	if (t == NULL)
		return NULL;

	while (1)
	{
		Token* q = NULL;
		if (*p == NULL)
		{
			TokenTree* node = new TokenTree();
			node->left = NULL;
			node->right = NULL;
			node->t.lexeme = t->lexeme;
			node->t.line_no = t->line_no;
			node->t.token_type = t->token_type;
			node->t.value_type = t->value_type;
			*p = node;

		}
//		else
//		{

			if (t->token_type == ID || t->token_type == NUM || t->token_type == REAL)
			{
				return t;
			}
			
			if (this->curStatement.RHS.fList.size() > 0) {
				Token t1;
				t1 = this->curStatement.RHS.fList.front();
				this->curStatement.RHS.fList.erase(this->curStatement.RHS.fList.begin());
				q = buildTree(&(*p)->right, &t1);
			}
			if (this->curStatement.RHS.fList.size() > 0) {
				Token t2;
				t2 = this->curStatement.RHS.fList.front();
				this->curStatement.RHS.fList.erase(this->curStatement.RHS.fList.begin());
				q = buildTree(&(*p)->left, &t2);
			}
			if (this->curStatement.RHS.fList.size() == 0)
				return q;

			return q;
//		}
	} //while loop
}

TokenTree *Parser::assignTypesinTree(TokenTree* ptr)
{
	Token* q;

/*
	if (ptr == NULL)
	{
		return NULL;
	}

	q = &(ptr->t);
	cout << "Processing: " << q->lexeme << ":" << q->token_type << "\n";

	if (ptr->right != NULL)
		return assignTypesinTree(ptr->right);

	if (ptr->left != NULL)
		return assignTypesinTree(ptr->left);
	
	//q = &(ptr->t);
	ptr->t.value_type = evaluateNodes(ptr, ptr->left, ptr->right);
	return ptr;
	*/

	if (ptr == NULL)
		return NULL;

//	ptr->t.value_type = evaluateNewNodes(ptr);
	traverseTree(ptr);
	return ptr;
}

void Parser::traverseTree(TokenTree* p)
{
	if (p == NULL)
		return;
	else
	{
		TokenType tt;
		
		if (p->t.lexeme.size() > 0)
		{
			tt = this->mt[p->t.lexeme];
			if (tt > 0) // we found the token in global variables
				p->t.value_type = tt;
		}

		if (p->t.token_type == NUM)
			p->t.value_type = INT;
		if (p->t.token_type == REALNUM)
			p->t.value_type = REAL;
		if (p->t.token_type == TR ||
			p->t.token_type == FA)
			p->t.value_type = BOO;

		//HERE
		traverseTree(p->right);

		traverseTree(p->left);

		//cout << p->t.line_no << " : Tree node : " << p->t.lexeme << " : " << p->t.token_type << ":" << p->t.value_type << ":" << "TT = " << tt << "\n";

		p->t.value_type = evaluateNewNodes(p);

		//cout << p->t.line_no << " : Tree node : " << p->t.lexeme << " : " << p->t.token_type << ":" << p->t.value_type << ":" << "TT = " << tt << "\n";

	}

}

TokenType Parser::normalizeTokenTypes(TokenType t)
{
	if (t == NUM)
		return INT;
	if (t == REALNUM)
		return REAL;
	if (t == TR ||
		t == FA ||
		t == BOO)
		return BOO;
	
	return t;

}

TokenType Parser::evaluateNewNodes(TokenTree* ptr)
{
	
	TokenTree* p;
	TokenTree* l;
	TokenTree* r;

	p = ptr;
	
	if (p == NULL)
		return UNKNOWN;

	if ((p->left == NULL) && (p->right == NULL)) //if both children are null, you are at leaf node
		return p->t.value_type;
	
	l = p->left;
	r = p->right;
	if ((l == NULL) || (r == NULL)) // if one child is null, you have a unary operator
	{
		if (isUnaryLogicOperator(p->t.token_type))
		{
			if (l != NULL)
				if (isUnaryLogicTokenType(l->t.value_type))
					return BOO;
			if (r != NULL)
				if (isUnaryLogicTokenType(r->t.value_type))
					return BOO;
			cout << "TYPE MISMATCH " << p->t.line_no << " C3\n";
			exit(0);
		}
	}
	// you have both L & R nodes - so, the token must be of binary type
	if (isBinaryMathOperator(p->t.token_type))
	{
		
		if ((this->normalizeTokenTypes(l->t.value_type) == this->normalizeTokenTypes(r->t.value_type)))
			return this->normalizeTokenTypes(l->t.value_type);
		if (l->t.value_type == UNKNOWN)
			return r->t.value_type;
		if (r->t.value_type == UNKNOWN)
			return l->t.value_type;

		cout << "TYPE MISMATCH " << p->t.line_no << " C2\n";
		exit(0);
	}

	if (isBinaryLogicOperator(p->t.token_type))
	{
	
		if ((this->normalizeTokenTypes(l->t.value_type) == this->normalizeTokenTypes(r->t.value_type)))
			return BOO;

		if (l->t.value_type == UNKNOWN)
			return BOO;
		if (r->t.value_type == UNKNOWN)
			return BOO;

		cout << "TYPE MISMATCH " << p->t.line_no << " C2\n";
		exit(0);
	}

}



TokenType Parser::evaluateNodes(TokenTree* p, TokenTree* l, TokenTree* r)
{
	if ((l == NULL) && (r == NULL)) //if both children are null, you are at leaf node
		return p->t.value_type;

	if ((l == NULL) || (r == NULL)) // if one child is null, you have a unary operator
	{
		if (isUnaryLogicOperator(p->t.token_type))
		{
			if (l != NULL)
				if (isUnaryLogicTokenType(l->t.token_type))
					return BOO;
			if (r != NULL)
				if (isUnaryLogicTokenType(r->t.token_type))
					return BOO;
			cout << "TYPE MISMATCH " << p->t.line_no << " C3\n";
			exit(0);
		}
	}
	// you have both L & R nodes - so, the token must be of binary type
	if (isBinaryMathOperator(p->t.token_type) )
	{
		if ((l->t.token_type == r->t.token_type))
			return l->t.token_type;
		if (l->t.token_type == UNKNOWN)
			return r->t.token_type;
		if (r->t.token_type == UNKNOWN)
			return l->t.token_type;
		
		cout << "TYPE MISMATCH" << p->t.line_no << " C2\n";
		exit(0);
	}

	if (isBinaryLogicOperator(p->t.token_type))
	{
	
		if ((l->t.token_type == r->t.token_type))
			return BOO;
		if (l->t.token_type == UNKNOWN)
			return BOO;
		if (r->t.token_type == UNKNOWN)
			return BOO;

		cout << "TYPE MISMATCH" << p->t.line_no << " C2\n";
		exit(0);
	}

}

/*
 * Function Completed
 * Acts as our assignment statement parser
 */
int Parser::parse_assstmt() {
#ifdef DEBUG
	cout << "Entered Parse Assignment Statement" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type != ID) {
		syntax_error();
	}
	//LHS
	this->curStatement.statementType = ASSIGNMENT;
	this->curStatement.LHS = token;
	// Do something with ID
	token = lexer.GetToken();
	if (token.token_type != EQUAL) {
		syntax_error();
	}
	else
	{
		this->curStatement.assignment = token.token_type;
	}
	this->curStatement.RHS.pTree = nullptr;

	parse_expression();
	token = lexer.GetToken();
	if (token.token_type != SEMICOLON) {
		syntax_error();
	}
	if (this->curStatement.RHS.fList.size() > 0) //Check
	{
		Token tt1;
		tt1 = this->curStatement.RHS.fList.front();
		this->curStatement.RHS.fList.erase(this->curStatement.RHS.fList.begin());
		this->buildTree(&(this->curStatement.RHS.pTree), &tt1);
	}
	this->statementList.push_back(this->curStatement);
	this->curStatement.initialize();
	
	return 0;
}

/*
 * Completed Function
 * Acts as our expression handling.
 */
int Parser::parse_expression() {
#ifdef DEBUG
	cout << "Entered Parse Expression" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type == NOT) {
		
		lexer.UngetToken(token);
		parse_unaryOperator();
		parse_expression();
	}
	else if (token.token_type == PLUS || token.token_type == MINUS || token.token_type == MULT || token.token_type == DIV) {
		
		lexer.UngetToken(token);
		parse_binaryOperator();
		parse_expression();
		parse_expression();
	}
	else if (token.token_type == GREATER || token.token_type == LESS || token.token_type == GTEQ || token.token_type == LTEQ || token.token_type == EQUAL || token.token_type == NOTEQUAL) {
		
		lexer.UngetToken(token);
		parse_binaryOperator();
		parse_expression();
		parse_expression();
	}
	else if (token.token_type == ID || token.token_type == NUM || token.token_type == REALNUM || token.token_type == TR || token.token_type == FA) {
	
		lexer.UngetToken(token);
		parse_primary();
	}
	else {
		syntax_error();
	}

	return 0;
}

/*
 * Completed Function
 * Gets our NOT token
 */
int Parser::parse_unaryOperator() {
#ifdef DEBUG
	cout << "Entered Parse Unary Operator" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type != NOT) {
		syntax_error();
	}

	this->curStatement.RHS.oList.push_back(token);
	this->curStatement.RHS.fList.push_back(token);
	//this->addToTree(&(this->curStatement.RHS.pTree), &token);

	//Do something with the NOT

	return 0;
}

/*
 * Completed Function
 * Acts as our binary handler
 */
int Parser::parse_binaryOperator() {
#ifdef DEBUG
	cout << "Entered Binary Operator" << endl; //RHS Operators
#endif
	token = lexer.GetToken();
	if (token.token_type == PLUS || token.token_type == MINUS || token.token_type == MULT || token.token_type == DIV) {
		// Do something with these Tokens
		this->curStatement.RHS.oList.push_back(token);
		this->curStatement.RHS.fList.push_back(token);
		//this->addToTree(&(this->curStatement.RHS.pTree), &token);

	}
	else if (token.token_type == GREATER || token.token_type == LESS || token.token_type == GTEQ || token.token_type == LTEQ || token.token_type == EQUAL || token.token_type == NOTEQUAL) {
		// Do something with these Tokens
		this->curStatement.RHS.oList.push_back(token);
		this->curStatement.RHS.fList.push_back(token);
		//this->addToTree(&(this->curStatement.RHS.pTree), &token);

	}
	else {
		syntax_error();
	}

	return 0;
}

/*
 * Completed Function
 * Acts as our primary handler
 */
int Parser::parse_primary() {
#ifdef DEBUG
	cout << "Entered Parse Primary" << endl; //RHS variables
#endif
	token = lexer.GetToken();
	if (token.token_type == ID || token.token_type == NUM || token.token_type == REALNUM || token.token_type == TR || token.token_type == FA) {
		// Do something with these Tokens
		this->curStatement.RHS.vList.push_back(token);
		this->curStatement.RHS.fList.push_back(token);
		//this->addToTree(&(this->curStatement.RHS.pTree), &token);
	}
	else {
		syntax_error();
	}

	return 0;
}

/*
 * Completed Function
 * Acts as our If Statement handler
 */
int Parser::parse_ifstmt() {
#ifdef DEBUG
	cout << "Entered Parse If Statement" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type != IF) {
		syntax_error();
	}
	token = lexer.GetToken();
	if (token.token_type != LPAREN) {
		syntax_error();
	}
	this->curStatement.statementType = IFSTATEMENT;
	parse_expression();
	token = lexer.GetToken();
	if (token.token_type != RPAREN) {
		syntax_error();
	}
	
	if (this->curStatement.RHS.fList.size() > 0) //check
	{
		Token tt1;
		tt1 = this->curStatement.RHS.fList.front();
		this->curStatement.RHS.fList.erase(this->curStatement.RHS.fList.begin());
		this->buildTree(&(this->curStatement.RHS.pTree), &tt1);
	}
	this->statementList.push_back(this->curStatement);
	this->curStatement.initialize();
	parse_body();

	return 0;
}

/*
 * Completed Function
 * Acts as our While Statement handler
 */
int Parser::parse_whilestmt() {
#ifdef DEBUG
	cout << "Entered Parse While Statement" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type != WHILE) {
		syntax_error();
	}
	token = lexer.GetToken();
	if (token.token_type != LPAREN) {
		syntax_error();
	}
	parse_expression();
	token = lexer.GetToken();
	if (token.token_type != RPAREN) {
		syntax_error();
	}
	if (this->curStatement.RHS.fList.size() > 0) //check
	{
		Token tt1;
		tt1 = this->curStatement.RHS.fList.front();
		this->curStatement.RHS.fList.erase(this->curStatement.RHS.fList.begin());
		this->buildTree(&(this->curStatement.RHS.pTree), &tt1); //7PM
	}
	this->statementList.push_back(this->curStatement);
	this->curStatement.initialize();
	parse_body();

	return 0;
}

/*
 * Completed Function
 * Acts as out Switch Statement handler
 */
int Parser::parse_switchstmt() {
#ifdef DEBUG
	cout << "Entered Switch Statement" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type != SWITCH) {
		syntax_error();
	}
	token = lexer.GetToken();
	if (token.token_type != LPAREN) {
		syntax_error();
	}
	parse_expression();
	token = lexer.GetToken();
	if (token.token_type != RPAREN) {
		syntax_error();
	}
	token = lexer.GetToken();
	if (token.token_type != LBRACE) {
		syntax_error();
	}
	if (this->curStatement.RHS.fList.size() > 0) //check
	{
		Token tt1;
		tt1 = this->curStatement.RHS.fList.front();
		this->curStatement.RHS.fList.erase(this->curStatement.RHS.fList.begin());
		this->buildTree(&(this->curStatement.RHS.pTree), &tt1);
	}
	this->statementList.push_back(this->curStatement);
	this->curStatement.initialize();
	parse_caselist();
	token = lexer.GetToken();
	if (token.token_type != RBRACE) {
		syntax_error();
	}

	return 0;
}

/*
 * TODO
 */
int Parser::parse_caselist() {
#ifdef DEBUG
	cout << "Entered Parse Case List" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type == CASE) {
		while (token.token_type == CASE) {
			lexer.UngetToken(token);
			parse_case();
			token = lexer.GetToken();
		}
		lexer.UngetToken(token);
	}
	else {
		syntax_error();
	}

	return 0;
}

int Parser::parse_case() {
#ifdef DEBUG
	cout << "Entered Parse Case" << endl;
#endif
	token = lexer.GetToken();
	if (token.token_type != CASE) {
		syntax_error();
	}
	token = lexer.GetToken();
	if (token.token_type != NUM) {
		syntax_error();
	}
	// Do something with this
	token = lexer.GetToken();
	if (token.token_type != COLON) {
		syntax_error();
	}
	parse_body();

	return 0;
}

int Parser::assign_types()
{
	Token t;
	int count = (int)this->statementList.size();
	for (int i = 0; i < count; i++)
	{
		switch (this->statementList[i].statementType)
		{
		case ASSIGNMENT:
			if (this->statementList[i].LHS.token_type == ID)
			{// If the variable matches the global list, change the value type
				t = this->statementList[i].LHS;
				for (int k = 0; k < (int)this->lexer.tList.size(); k++)
				{
					if (this->lexer.tList[k].lexeme == t.lexeme)
					{
						this->statementList[i].LHS.value_type = this->lexer.tList[k].value_type;
						
						break;
					} 
				}
			}
			else
			{
				cout << "Non-ID is on LHS of an assignment\n";
				syntax_error();
				return 0;
			}
			if (t.token_type == ID)
			{
				//check if RHS has any constants - NUM, REALNUM, etc..
				TokenType tempTT = UNKNOWN;

				for (int j = 0; j < (int)this->statementList[i].RHS.vList.size(); j++)
				{
					if (tempTT == UNKNOWN &&
						tempTT != this->statementList[i].RHS.vList[j].value_type)
							tempTT = this->statementList[i].RHS.vList[j].value_type;
				}

				for (int j = 0; j < (int)this->statementList[i].RHS.vList.size(); j++)
				{
					if (this->statementList[i].RHS.vList[j].value_type == UNKNOWN)
						this->statementList[i].RHS.vList[j].value_type = tempTT;
				}


				if (this->statementList[i].LHS.value_type == UNKNOWN)
					this->statementList[i].LHS.value_type = tempTT;

				if (!(mt.find(t.lexeme) != mt.end()))
				{
					mt[t.lexeme] = this->statementList[i].LHS.value_type;
				}
				else if (mt[t.lexeme] == UNKNOWN && this->statementList[i].LHS.value_type != UNKNOWN)
				{
					mt[t.lexeme] = this->statementList[i].LHS.value_type;
					loopback(this->statementList[i].LHS);
				}
				this->symbolTable.push_back(this->statementList[i].LHS);
			}
			for (int j = 0; j < (int)this->statementList[i].RHS.vList.size(); j++)
			{// If the variable matches the global list, change the value type
				t = this->statementList[i].RHS.vList[j];
				for (int k = 0; k < (int)this->lexer.tList.size(); k++)
				{
					if (this->lexer.tList[k].lexeme == t.lexeme)
					{
						this->statementList[i].RHS.vList[j].value_type = this->lexer.tList[k].value_type;
						break;
					}
				}
				if (t.token_type == ID) {
					if (!(mt.find(t.lexeme) != mt.end())) //if key doesn't exist in map table, insert
					{
						mt[t.lexeme] = this->statementList[i].RHS.vList[j].value_type;
					}
					else if (mt[t.lexeme] == UNKNOWN && this->statementList[i].RHS.vList[j].value_type != UNKNOWN)
					{
						mt[t.lexeme] = this->statementList[i].RHS.vList[j].value_type;
						loopback(t);
					}

					this->symbolTable.push_back(this->statementList[i].RHS.vList[j]);
				}
					
			}


			break;


		case IFSTATEMENT:
		case WHILESTATEMENT:
		case SWITCHSTATEMENT:
			
			for (int j = 0; j < (int)this->statementList[i].RHS.vList.size(); j++)
			{// do something with RHS variables and operators
				t = this->statementList[i].RHS.vList[j];
				for (int k = 0; k < (int)this->lexer.tList.size(); k++)
				{
					if (this->lexer.tList[k].lexeme == t.lexeme)
					{
						this->statementList[i].RHS.vList[j].value_type = this->lexer.tList[k].value_type;
						break;
					}
				}
				if (t.token_type == ID) {
					if (!(mt.find(t.lexeme) != mt.end()))
					{
						mt[t.lexeme] = this->statementList[i].RHS.vList[j].value_type;
					}
					else if (mt[t.lexeme] == UNKNOWN && this->statementList[i].RHS.vList[j].value_type != UNKNOWN)
					{
						mt[t.lexeme] = this->statementList[i].RHS.vList[j].value_type;
						loopback(t);
					}

					this->symbolTable.push_back(this->statementList[i].RHS.vList[j]);
				}
			}

			break;
		
		case CASESTATEMENT: break;
		default:
			return 0;
		}
	}
	return 0;
}

void Parser::loopback(Token t)
{
	for (int i = 0; i < this->statementList.size(); i++)
	{
		Statement st = this->statementList[i];
	
			for (int j = 0; j < this->statementList[i].RHS.vList.size(); j++)
			{
				if (this->statementList[i].RHS.vList[j].lexeme == t.lexeme)
				{
					this->statementList[i].RHS.vList[j].value_type = t.value_type;
					for (int k = 0; k < this->statementList[i].RHS.vList.size(); k++)
					{
						if (this->statementList[i].RHS.vList[k].value_type != t.value_type)
						{
							this->statementList[i].RHS.vList[k].value_type = t.value_type;
							this->mt[this->statementList[i].RHS.vList[k].lexeme] = t.value_type;
							loopback(this->statementList[i].RHS.vList[k]);
						}
					}
					if (st.statementType == ASSIGNMENT)
					{
						if (this->statementList[i].LHS.value_type != t.value_type)
						{
							this->statementList[i].LHS.value_type = t.value_type;
							this->mt[this->statementList[i].LHS.lexeme] = t.value_type;
							loopback(this->statementList[i].LHS);
						}
					}
				}
			}
				
		
	}
	return;
}

int Parser::assign_types2()
{
	Token t;
	int count = (int)this->statementList.size();
	for (int m = 0; m < count; m++)
	{
		int i = count - (m + 1);

		switch (this->statementList[i].statementType)
		{
		case ASSIGNMENT:
			if (this->statementList[i].LHS.token_type == ID)
			{// If the variable matches the global list, change the value type
				t = this->statementList[i].LHS;
				for (int k = 0; k < (int)this->lexer.tList.size(); k++)
				{
					if (this->lexer.tList[k].lexeme == t.lexeme)
					{
						this->statementList[i].LHS.value_type = this->lexer.tList[k].value_type;

						break;
					}
				}
			}
			else
			{
				cout << "Non-ID is on LHS of an assignment\n";
				syntax_error();
				return 0;
			}
			if (t.token_type == ID)
			{
				//check if RHS has any constants - NUM, REALNUM, etc..
				TokenType tempTT = UNKNOWN;

				for (int j = 0; j < (int)this->statementList[i].RHS.vList.size(); j++)
				{
					if (tempTT == UNKNOWN &&
						tempTT != this->statementList[i].RHS.vList[j].value_type)
						tempTT = this->statementList[i].RHS.vList[j].value_type;
				}

				for (int j = 0; j < (int)this->statementList[i].RHS.vList.size(); j++)
				{
					if (this->statementList[i].RHS.vList[j].value_type == UNKNOWN)
						this->statementList[i].RHS.vList[j].value_type = tempTT;
				}


				if (this->statementList[i].LHS.value_type == UNKNOWN)
					this->statementList[i].LHS.value_type = tempTT;

				if (!(mt2.find(t.lexeme) != mt2.end()))
				{
					mt2[t.lexeme] = this->statementList[i].LHS.value_type;
				}
				this->symbolTable.push_back(this->statementList[i].LHS);
			}
			for (int j = 0; j < (int)this->statementList[i].RHS.vList.size(); j++)
			{// If the variable matches the global list, change the value type
				t = this->statementList[i].RHS.vList[j];
				for (int k = 0; k < (int)this->lexer.tList.size(); k++)
				{
					if (this->lexer.tList[k].lexeme == t.lexeme)
					{
						this->statementList[i].RHS.vList[j].value_type = this->lexer.tList[k].value_type;
						break;
					}
				}
				if (t.token_type == ID) {
					if (!(mt2.find(t.lexeme) != mt2.end())) //if key doesn't exist in map table, insert
					{
						mt2[t.lexeme] = this->statementList[i].RHS.vList[j].value_type;
					}
					this->symbolTable.push_back(this->statementList[i].RHS.vList[j]);
				}

			}


			break;


		case IFSTATEMENT:
		case WHILESTATEMENT:
		case SWITCHSTATEMENT:

			for (int j = 0; j < (int)this->statementList[i].RHS.vList.size(); j++)
			{// do something with RHS variables and operators
				t = this->statementList[i].RHS.vList[j];
				for (int k = 0; k < (int)this->lexer.tList.size(); k++)
				{
					if (this->lexer.tList[k].lexeme == t.lexeme)
					{
						this->statementList[i].RHS.vList[j].value_type = this->lexer.tList[k].value_type;
						break;
					}
				}
				if (t.token_type == ID) {
					if (!(mt2.find(t.lexeme) != mt2.end()))
					{
						mt2[t.lexeme] = this->statementList[i].RHS.vList[j].value_type;
					}
					this->symbolTable.push_back(this->statementList[i].RHS.vList[j]);
				}
			}

			break;

		case CASESTATEMENT: break;
		default:
			return 0;
		}
	}
	return 0;
}


TokenType Parser::evaluateRHSType(int index)
{/*
	Token t1, t2, o;
	
	if ((this->statementList[index].RHS.vList.size() == 2) && (this->statementList[index].RHS.oList.size() == 1))
	{
		t1 = this->statementList[index].RHS.vList[0];
		t2 = this->statementList[index].RHS.vList[1];
		o = this->statementList[index].RHS.oList[0];
	}
	else
	{
		return UNKNOWN;
	}
	
	if (isBinaryLogicOperator(o.token_type))
	{
		if (t1.token_type == t2.token_type ||
			t1.token_type == UNKNOWN ||
			t2.token_type == UNKNOWN)
		{
			return o.token_type;
		}
	}
	else if (isBinaryMathOperator(o.token_type))
	{
	}
	*/
	return UNKNOWN;
}



bool Parser::isBinaryMathOperator(TokenType tt)
{
	if (tt == PLUS ||
		tt == MINUS ||
		tt == MULT ||
		tt == DIV )
		return true;

	return false;
}


bool Parser::isBinaryLogicOperator(TokenType tt)
{
	if (tt == GTEQ ||
		tt == GREATER ||
		tt == LESS ||
		tt == LTEQ ||
		tt == EQUAL ||
		tt == NOTEQUAL)
		return true;

	return false;
}

bool Parser::isUnaryLogicOperator(TokenType tt)
{
	if (tt == NOT)
		return true;

	return false;
}

bool Parser::isUnaryLogicTokenType(TokenType tt)
{
	if (tt == BOO ||
		tt == TR ||
		tt == FA)
			return true;

	return false;
}


TokenType Parser::evaluateExpression(std::vector<Token> oList, std::vector<Token> vList)
{
	TokenType tt;

	if (oList.size() == 0) // if no operator, the value_type of the single token needs to be returned
		if (vList.size() == 1)
			return vList[0].value_type;
	
	for (int i = 0; i < oList.size(); i++)
	{
		Token t = oList[i];
		if (isBinaryLogicOperator(t.token_type))
		{// if there is a single binary operator, the return type is always boolean; but all vList tokens have to be same or unknown tokentype
			for (int j = 0; j < vList.size(); j++)
			{
				TokenType tt1;
			}
		}
	}
	tt = UNKNOWN;
	return tt;
}
bool Parser::validate_c1()
{
	Token t;
	int count = (int)this->statementList.size();
	for (int i = 0; i < count; i++)
	{
		if (this->statementList[i].statementType == ASSIGNMENT)
		{
			if (this->statementList[i].LHS.token_type == ID)
			{// If the variable matches the global list, change the value type

				evaluateRHSType(i);
				//t = evaluateExpression(this->statementList[i].RHS.oList, this->statementList[i].RHS.vList);
			}
			else
			{
				cout << "Non-ID is on LHS of an assignment\n";
				syntax_error();
				return 0;
			}

		}

	}// for all statements
	return false;
}

bool Parser::validate_c2()
{
	return false;
}

bool Parser::validate_c3()
{
	return false;
}

bool Parser::validate_c4()
{
	return false;
}

bool Parser::validate_c5()
{
	return false;
}

string Parser::tokenTypeToString(TokenType tt)
{
	switch (tt) {
	case REAL:
	case REALNUM:
		return "real";

	case INT:
	case NUM:
		return "int";

	case UNKNOWN:
		return "?";

	case BOO:
	case TR:
	case FA:
		return "bool";

	default:
		return "";


	}
}

void Parser::print()
{
	std::map<string, TokenType> map;
	string strUV;

	for (int i = 0; i < this->statementList.size(); i++)
	{
		strUV = "";
		Statement st = this->statementList[i];

		if (st.statementType == SWITCHSTATEMENT ||
			st.statementType == IFSTATEMENT ||
			st.statementType == WHILESTATEMENT)
		{
			for (int j = 0; j < st.RHS.vList.size(); j++)
			{
				if (st.RHS.vList[j].token_type == ID)
					if (!(map.find(st.RHS.vList[j].lexeme) != map.end()))
					{
						map[st.RHS.vList[j].lexeme] = st.RHS.vList[j].value_type;
						if (st.RHS.vList[j].value_type != UNKNOWN)
							cout << st.RHS.vList[j].lexeme << ": " << tokenTypeToString(st.RHS.vList[j].value_type) << " #\n";
						else
						{
							if (strUV.length() > 0)
								strUV += ", " + st.RHS.vList[j].lexeme;
							else
								strUV = st.RHS.vList[j].lexeme;
						}
					}
					
			}
			if (strUV.length() > 0)
			{ //there are some unknown variables ... 
				strUV += ": " + tokenTypeToString(UNKNOWN) + " #\n";
				cout << strUV;
			}
		}
		if (st.statementType == ASSIGNMENT)
		{
			if ((st.LHS.value_type == UNKNOWN) && (st.RHS.pTree->t.value_type == UNKNOWN))
			{
				if (!(map.find(st.LHS.lexeme) != map.end()))
				{
					map[st.LHS.lexeme] = st.LHS.value_type; //string that is already printed is stored in map table
					//cout << st.LHS.lexeme << ": " << tokenTypeToString(map[st.LHS.lexeme]) << " #\n";
					strUV = st.LHS.lexeme;
				}
				for (int j = 0; j < st.RHS.vList.size(); j++)
				{
					if (st.RHS.vList[j].token_type == ID)
						if (!(map.find(st.RHS.vList[j].lexeme) != map.end()))
						{
							map[st.RHS.vList[j].lexeme] = st.RHS.vList[j].value_type;
							if (st.RHS.vList[j].value_type != UNKNOWN)
								cout << st.RHS.vList[j].lexeme << ": " << tokenTypeToString(st.RHS.vList[j].value_type) << " #\n";
							else {
								if (strUV.length() > 0)
									strUV += ", " + st.RHS.vList[j].lexeme;
								else
									strUV = st.RHS.vList[j].lexeme;
							}
			
						}
				}
				if (strUV.length() > 0)
				{
					strUV += ": " + tokenTypeToString(UNKNOWN) + " #\n";
					cout << strUV;
				}
			}
			if ((st.LHS.value_type != UNKNOWN))
			{
				if (!(map.find(st.LHS.lexeme) != map.end()))
				{
					map[st.LHS.lexeme] = st.LHS.value_type;
					cout << st.LHS.lexeme << ": " << tokenTypeToString(map[st.LHS.lexeme]) << " #\n";
				}

				for (int j = 0; j < st.RHS.vList.size(); j++)
				{
					if (st.RHS.vList[j].token_type == ID)
						if (!(map.find(st.RHS.vList[j].lexeme) != map.end()))
						{
							map[st.RHS.vList[j].lexeme] = st.RHS.vList[j].value_type;
							cout << st.RHS.vList[j].lexeme << ": " << tokenTypeToString(st.RHS.vList[j].value_type) << " #\n";
						}
				}
			}
			else if ((st.LHS.value_type == UNKNOWN) && (st.RHS.pTree->t.value_type != UNKNOWN))
			{
				for (int j = 0; j < st.RHS.vList.size(); j++)
				{
					if (st.RHS.vList[j].token_type == ID)
						if (!(map.find(st.RHS.vList[j].lexeme) != map.end()))
						{
							map[st.RHS.vList[j].lexeme] = st.RHS.vList[j].value_type;
							cout << st.RHS.vList[j].lexeme << ": " << tokenTypeToString(st.RHS.vList[j].value_type) << " #\n";
						}
				}
				if (!(map.find(st.LHS.lexeme) != map.end()))
				{
					if (st.RHS.pTree != NULL)
						map[st.LHS.lexeme] = st.RHS.pTree->t.value_type;
					else
						map[st.LHS.lexeme] = st.RHS.vList[0].value_type;

					cout << st.LHS.lexeme << ": " << tokenTypeToString(map[st.LHS.lexeme]) << " #\n";
				}
			}

			if (st.RHS.pTree)
				if ((st.LHS.value_type != UNKNOWN) && (st.RHS.pTree->t.value_type != UNKNOWN))
				{
					if (!(map.find(st.LHS.lexeme) != map.end()))
					{
						map[st.LHS.lexeme] = st.LHS.value_type;
						cout << st.LHS.lexeme << ": " << tokenTypeToString(st.RHS.vList[0].value_type) << " #\n";
					}
					for (int j = 0; j < st.RHS.vList.size(); j++)
					{
						if (st.RHS.vList[j].token_type == ID)
							if (!(map.find(st.RHS.vList[j].lexeme) != map.end()))
							{
								map[st.RHS.vList[j].lexeme] = st.RHS.vList[j].value_type;
								cout << st.RHS.vList[j].lexeme << ": " << tokenTypeToString(st.RHS.vList[j].value_type) << " #\n";
							}
					}

				}
		} 

	}
	return;

}

int main() {
#ifdef DEBUG
	cout << "Entered Main" << endl;
#endif

	vector<string> output;
	
	int i;
	bool flag = false;
	Parser* parseProgram = new Parser();
	i = parseProgram->parse_program();
	i = parseProgram->assign_types();
	//i = parseProgram->assign_types2();



	for (int i = 0; i < parseProgram->statementList.size(); i++)
	{
		Statement st = parseProgram->statementList[i];
		
		if (st.statementType == SWITCHSTATEMENT)
		{
			for (int j = 0; j < st.RHS.vList.size(); j++)
			{
				if (!((parseProgram->normalizeTokenTypes(st.RHS.vList[j].value_type) == INT) || (st.RHS.vList[j].value_type == UNKNOWN)))
				{
					cout << "TYPE MISMATCH " << st.RHS.vList[0].line_no << " C5\n";
					exit(0);
				}

			}
			//force all variables inside the switch statement to an int type
			for (int j = 0; j < st.RHS.vList.size(); j++)
			{
				if (st.RHS.vList[j].token_type == ID && 
					st.RHS.vList[j].value_type == UNKNOWN)
						parseProgram->statementList[i].RHS.vList[j].value_type = INT;
			}
			for (int j = 0; j < st.RHS.fList.size(); j++)
			{
				if (st.RHS.fList[j].token_type == ID &&
					st.RHS.fList[j].value_type == UNKNOWN)
					parseProgram->statementList[i].RHS.fList[j].value_type = INT;
			}
		}

	}



	TokenTree *tt;
	for (int i = 0; i < parseProgram->statementList.size(); i++)
	{
		tt = parseProgram->assignTypesinTree(parseProgram->statementList[i].RHS.pTree);
	}






	for (int i = 0; i < parseProgram->statementList.size(); i++)
	{
		Statement st = parseProgram->statementList[i];
		if (st.statementType == ASSIGNMENT)
		{
			if (st.LHS.value_type == UNKNOWN)
				st.LHS.value_type = st.RHS.pTree->t.value_type;

			if (st.RHS.pTree)
				if (parseProgram->normalizeTokenTypes(st.LHS.value_type) != parseProgram->normalizeTokenTypes(st.RHS.pTree->t.value_type))
			{
				cout << "TYPE MISMATCH " << st.LHS.line_no << " C1\n";
				exit(0);
			}
			
		}
		if (st.statementType == IFSTATEMENT || st.statementType == WHILESTATEMENT)
		{
			if ((parseProgram->normalizeTokenTypes(st.RHS.pTree->t.value_type) != BOO) &&
				(st.RHS.pTree->t.value_type != UNKNOWN))
			{
				cout << "TYPE MISMATCH " << st.RHS.vList[0].line_no << " C4\n";
				exit(0);
			}

		}

		

	}

	parseProgram->print();

	//cout << "\nEnd of Program" << endl;
	return 0;
}
