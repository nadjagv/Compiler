#pragma once

#include <vector>
#include <stack>
#include "IR.h"

typedef std::stack<Variable*> VariableStack;
typedef std::vector<std::vector<int>> InterferenceMatrix;


// InterferenceGraph class
class InterferenceGraph
{
public:
	InterferenceMatrix interMatrix;
	VariableStack varStack;  // same as list of instructions, just in form of stack
	Variables& regVars;  // register variables

	/* Constructor, destructor */
	InterferenceGraph(Variables& vars);
	~InterferenceGraph();

	/* Resizes matrix to the passed size. */
	void resizeInterferenceMatrix(size_t size);

	/* Applies reg to the variable that has varPos for it's position. */
	void applyRegToVar(int varPos, Regs reg);

	

							 /* Prints interference matrix to the console. */
	void printMatrix();

	/* Allocates real registers to variables according to the interference. */
	bool resourceAllocation();

	/* Creates interference graph (matrix) from instructions. */
	void buildGraph(Instructions& instructions);

	/* Creates stack from list of register variables. */
	void buildVarStack();

	/*returns color, returns -1 if error*/
	int getColor(Variable* notColoredVariable);

	/*does resource allocation*/
	bool doResourceAllocation();
};

