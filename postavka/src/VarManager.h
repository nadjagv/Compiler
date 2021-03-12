#pragma once
#include "IR.h"
#include "Token.h"

using namespace std;
/*class for managing variables*/
class VarManager
{
public:
	Variables memVars;
	Variables regVars;
	int numVars;

	VarManager();
	/*checks if memory variable is already defined*/
	Variable* chackMemVar(string name);

	/*checks if register variable is already defined*/
	Variable* chackRegVar(string name);

	/*adds memory variable*/
	void addMemVar(Token& t, Token& v);

	/*adds register variable*/
	void addRegVar(Token& t);

	/*returns data section string*/
	string getDataSection();
};
