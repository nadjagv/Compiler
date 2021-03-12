
#include "IR.h"
#include "Types.h"
#include"Token.h"
#include <vector>

using namespace std;

/*label class*/
class Label
{
public:
	string name;
	int position;
	Label(string n, int p) : name(n), position(p) {}
};

typedef vector<Label> Labels;

/*class for managing labels*/
class LblManager 
{
	public:
		Labels labels;
		string currentLabel;

		/*check if label is defined*/
		bool checkLabel(string name);
		/*adds label if not already added*/
		void addLabel(Label l);
		/*returns label position*/
		int getPosFromLabel(string name);
};