#pragma once

#include "LexicalAnalysis.h"
#include "FuncManager.h"
#include "VarManager.h"
#include "IR.h"
#include "Token.h"

class SyntaxAnalysis
{
public:

	/**
	* Constructor
	*/
	int instructionCounter;
	Instructions instructions;
	LexicalAnalysis& lexicalAnalysis;
	FuncManager funcMng;
	LblManager lblMng;
	VarManager varMng;
	SyntaxAnalysis(Instructions& ins, LexicalAnalysis& lex, FuncManager& f, LblManager& l, VarManager& v);

	/*method for creating instruction*/
	void createInstruction(InstructionType type, vector<Token>& dst, vector<Token>& src);

	void fillSuccesors();
	void fillPredecessors();
	void createMIPSFile(string filePath);

	/**
	* Method which performs lexical analysis
	*/
	bool Do();

private:
	/**
	* Prints the error message, and token that caused the syntax error
	*/
	void printSyntaxError(Token token);

	/**
	* Prints the token info
	*/
	void printTokenInfo(Token token);

	/**
	* Eats the current token if its type is "t"
	* otherwise reports syntax error
	*
	* param[in] - t - the expected token type
	*/
	void eat(TokenType t);

	/**
	* Returns the next token from the token list
	*/
	Token getNextToken();

	/**
	 * Nonterminal Q
	 */
	void Q();

	/**
	 * Nonterminal S
	 */
	void S();

	/**
	 * Nonterminal L
	 */
	void L();

	/**
	 * Nonterminal E
	 */
	void E();


	/**
	* Syntax error indicator
	*/
	bool errorFound;

	/**
	* Iterator to the token list which represents the output of the lexical analysis
	*/
	TokenList::iterator tokenIterator;

	/**
	* Current token that is being analyzed
	*/
	Token currentToken;
};
