#include "VarManager.h"
#include <regex>
#include <sstream>

VarManager::VarManager() : numVars(0) {};

Variable* VarManager::chackMemVar(string name) {
	Variables::iterator it;
	for (it = memVars.begin(); it != memVars.end(); it++) {
		if ((*it)->getName() == name) {
			return *it;
		}
	}
	return NULL;
}

Variable* VarManager::chackRegVar(string name) {
	Variables::iterator it;
	for (it = regVars.begin(); it != regVars.end(); it++) {
		if ((*it)->getName() == name) {
			return *it;
		}
	}
	return NULL;
}

void VarManager::addMemVar(Token& t, Token& v) {
	if (chackMemVar(t.getValue()) != NULL) {
		throw runtime_error("Memory variable " +t.getValue() + " is already defined.");
	}

	if (regex_match(t.getValue(), regex("m[0-9]+")) == false){
		throw runtime_error("Name of memory variable " + t.getValue() + " isn't correct.");
	}
		
	Variable* var = new Variable(t.getValue(), 0, Variable::MEM_VAR, stoi(v.getValue()));
	memVars.push_back(var);
}

void VarManager::addRegVar(Token& t) {
	if (chackRegVar(t.getValue()) != NULL) {
		throw runtime_error("Register variable " + t.getValue() + " is already defined.");
	}

	if (regex_match(t.getValue(), regex("r[0-9]+")) == false) {
		throw runtime_error("Name of memory variable " + t.getValue() + " isn't correct.");
	}

	Variable* var = new Variable(t.getValue(), numVars, Variable::REG_VAR, 0);
	regVars.push_back(var);
	numVars++;
}

string VarManager::getDataSection()
{
	stringstream result;
	result << "\n.data\n";

	Variables::iterator it = memVars.begin();
	while (it != memVars.end())
	{
		result << (*it)->getName() << ": " << ".word "<< (*it)->getValue() << "\n";
		it++;
	}
	return result.str();
}
