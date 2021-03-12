#pragma once
#include "LblManager.h"

typedef vector<string> Functions;

/*class for managing functions*/
class FuncManager
{
public:
	Functions functions;
	LblManager lblM;
	FuncManager(LblManager l);

	/*check if function is alredy defined*/
	bool checkFunc(string name);
	/*adds function to vector of functions*/
	void addFunc(Token& t, int position);
	/*returns globl section string*/
	string getGloblSection();
};