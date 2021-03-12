#include "LblManager.h"
#include <regex>

using namespace std;

bool LblManager::checkLabel(string name) {
	Labels::iterator it;
	for (it = labels.begin(); it != labels.end(); it++) {
		if (it->name == name) {
			return true;
		}
	}
	return false;
}

void LblManager::addLabel(Label l) {
	if (checkLabel(l.name))
		throw runtime_error("Label " + l.name + " is already defined.");

	if (regex_match(l.name, regex("[A-Za-z]+[A-Za-z0-9]*")) == false)
		throw runtime_error("Name of label " + l.name + " isn't correct.");

	labels.push_back(l);
	currentLabel = l.name;
}

int LblManager::getPosFromLabel(string name) {
	Labels::iterator it;
	if (!checkLabel(name))
		throw runtime_error("Label " + name + " not defined.");

	for (it = labels.begin(); it != labels.end(); it++) {
		if (it->name == name) {
			return it->position;
		}
	}
	return -1;
}