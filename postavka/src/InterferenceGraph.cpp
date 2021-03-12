#include "InterferenceGraph.h"
#include <iomanip>
#include "Constants.h"

using namespace std;

InterferenceGraph::~InterferenceGraph(){}

InterferenceGraph::InterferenceGraph(Variables& vars): regVars(vars){}

/* Resizes matrix to the passed size. */
void InterferenceGraph::resizeInterferenceMatrix(size_t size)
{
	interMatrix.resize(size);
	int i;

	for (i = 0; i < interMatrix.size(); ++i)
		interMatrix[i].resize(size);
}

/* Creates interference graph (matrix) from instructions. */
void InterferenceGraph::buildGraph(Instructions& instructions)
{
	// Initialize interference matrix to the size*size of reg vars
	resizeInterferenceMatrix(regVars.size());

	// In every instructions, for all defined variables add
	// interference between defined variable and all out
	// variables from instruction.
	Instructions::iterator it = instructions.begin();
	for (it = instructions.begin(); it!= instructions.end(); it++)
	{
		Variables::iterator defVarIt = (*it)->m_def.begin();
		for (defVarIt = (*it)->m_def.begin(); defVarIt != (*it)->m_def.end(); defVarIt++)
		{
			Variables::iterator outVarIter = (*it)->m_out.begin();
			for (outVarIter = (*it)->m_out.begin(); outVarIter != (*it)->m_out.end(); outVarIter++)
			{

				int outPos = (*outVarIter)->getPosition();
				int defPos = (*defVarIt)->getPosition();

				// no interference with itself
				if (defPos == outPos)
					interMatrix[defPos][outPos] = interMatrix[outPos][defPos] = __EMPTY__;
				else
					interMatrix[defPos][outPos] = interMatrix[outPos][defPos] = __INTERFERENCE__;
			}
		}
	}
}


/* Creates stack from list of register variables. */
void InterferenceGraph::buildVarStack()
{
	Variables::iterator it;
	for (auto it = regVars.begin(); it != regVars.end(); it++)
		varStack.push(*it);
}

/* Applies reg to the variable that has varPos for it's position. */
void InterferenceGraph::applyRegToVar(int varPos, Regs reg)
{
	Variables::iterator it = regVars.begin();
	while (it != regVars.end())
	{
		if ((*it)->getPosition() == varPos)
			(*it)->setAssignment(reg);
		it++;
	}
}

/* Prints interference matrix to the console. */
void InterferenceGraph::printMatrix()
{
	printf("    ");
	for (int i = 0; i < interMatrix.size(); ++i)
		printf("R%-3d", i);

	cout << endl;

	for (int i = 0; i < interMatrix.size(); ++i)
	{
		printf("R%d  ", i);

		for (int j = 0; j < interMatrix[i].size(); ++j)
			printf("%-4d", interMatrix[i][j]);

		cout << endl;
	}
}


Variables save;

/* Allocates real registers to variables according to the interference. */
int InterferenceGraph::getColor(Variable* notColoredVariable) {
	Variables::iterator iter;
	Variables temp;

	// get variable from stack which are interference with notColoredVariable
	for (iter = save.begin(); iter != save.end(); iter++) {
		Variable* variable = (*iter);

		if (interMatrix[notColoredVariable->getPosition()][variable->getPosition()] == __INTERFERENCE__) {
			temp.push_back(variable);
		}
		else {
			// nothing
		}
	}

	// find diffrent color
	int color = 0;
	bool find;

	for (int color = 0; color < __REG_NUMBER__; color++) {

		find = true;

		for (iter = temp.begin(); iter != temp.end(); iter++) {
			if (color == (*iter)->getAssignment()) {
				find = false;
			}
		}

		if (find == true) {
			return color;
			break;
		}
	}

	return -1;
}

bool InterferenceGraph::doResourceAllocation() {

	Variable* currentVariable, * previusVariable;

	previusVariable = NULL;

	int counter = 0;

	while (varStack.size() > 0) {

		currentVariable = varStack.top();
		varStack.pop();

		save.push_back(currentVariable);

		if (previusVariable == NULL) {
			// first variable on stack
			currentVariable->setAssignment((Regs)counter);
		}
		else {
			// other varables
			int color = getColor(currentVariable);
			if (color == -1) {
				// actual spill                
				return false;
			}
			else {
				currentVariable->setAssignment((Regs)color);
			}
		}

		previusVariable = currentVariable;
	}

	regVars = save;

	return true;
}
