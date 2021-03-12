#include "FuncManager.h"
#include <regex>
#include <sstream>

FuncManager::FuncManager(LblManager l) : lblM(l) {}

bool FuncManager::checkFunc(string name) {
	Functions::iterator it;
	for (it = functions.begin(); it != functions.end(); it++) {
		if (*it == name) {
			return true;
		}
	}
	return false;
}

void FuncManager::addFunc(Token& t, int position) {
	if (regex_match(t.getValue(), regex("[A-Za-z]+[A-Za-z0-9]*")) == false)
		throw runtime_error("Name of function " + t.getValue() + " isn't correct.");

	if (checkFunc(t.getValue()))
		throw runtime_error("Function " + t.getValue() + " is already defined.");

	functions.push_back(t.getValue());
}

string FuncManager::getGloblSection()
{
	stringstream result;
	vector<string>::iterator itx = functions.begin();
	while (itx != functions.end())
	{
		result << ".globl " << *itx << "\n";
		itx++;
	}
	return result.str();
}