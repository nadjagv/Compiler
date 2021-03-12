#include "LivenessAnalysis.h"

bool variableExists(Variable* variable, Variables variables)
{
	Variables::iterator it;

	for (it = variables.begin(); it != variables.end(); it++)
	{
		if (*it == variable)
		{
			return true;
		}
	}

	return false;
}

void livenessAnalysis(Instructions instructions)
{
	bool doItAgain = true;

	while (doItAgain) {
		doItAgain = false;

		Instructions::reverse_iterator rit = instructions.rbegin();
		for (; rit != instructions.rend(); rit++) {
			// 0. in' = in, out' = out
			Variables& out = (*rit)->m_out;
			Variables& in = (*rit)->m_in;
			Variables newOut;
			Variables newIn;

			// 1. newOut = U succIn
			Instructions& successors = (*rit)->m_succ;
			Instructions::iterator succIt = successors.begin();
			for (; succIt != successors.end(); succIt++) {
				Variables& succIn = (*succIt)->m_in;
				newOut.insert(newOut.end(), succIn.begin(), succIn.end());
			}
			newOut.sort();
			newOut.unique();

			// 2. newIn = use U (out/def)
			Variables& use = (*rit)->m_use;
			Variables& def = (*rit)->m_def;

			Variables outMinusDef;
			Variables::iterator varIt = out.begin();
			for (; varIt != out.end(); varIt++) {
				if (!variableExists(*varIt, def)) {
					outMinusDef.push_back(*varIt);
				}
			}

			newIn = use;
			newIn.insert(newIn.end(), outMinusDef.begin(), outMinusDef.end());
			newIn.sort();
			newIn.unique();

			if (newIn != in || newOut != out) {
				doItAgain = true;
			}

			in = newIn;
			out = newOut;
		}
	}
}
