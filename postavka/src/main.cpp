#include <iostream>
#include <exception>


#include "SyntaxAnalysis.h"
#include "LivenessAnalysis.h"
#include "InterferenceGraph.h"

using namespace std;




int main()
{
	try
	{
		std::string fileName = ".\\..\\examples\\multiply.mavn";
		std::string outFileName = ".\\..\\examples\\multiply.s";
		bool retVal = false;

		Instructions instructions;
		LexicalAnalysis lex;
		VarManager varMng;
		LblManager lblMng;
		FuncManager funcMng(lblMng);


		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}

		SyntaxAnalysis syntax(instructions, lex, funcMng, lblMng, varMng);

		if (syntax.Do())
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			cout << "Exception! Syntax analysis failed!" << endl;
		}

		syntax.fillSuccesors();
		syntax.fillPredecessors();
		cout << endl << "====================================" << endl << endl << "Instruction list: " << endl << endl;
		printInstructions(syntax.instructions);

		// Do liveness analysis
		livenessAnalysis(syntax.instructions);

		cout << endl;
		cout << "Instruction list after liveness analysis: ";
		cout << endl << endl;
		printInstructions(syntax.instructions);


		// Create interference graph, do resource allocation
		InterferenceGraph interGraph(syntax.varMng.regVars);
		interGraph.buildGraph(syntax.instructions);
		interGraph.buildVarStack();

		// Print interference matrix
		cout << endl;
		cout << "Interference matrix:";
		cout << endl;
		interGraph.printMatrix();

		if (interGraph.doResourceAllocation())
		{
			cout << endl;
			cout << "Resource allocation successful!";
			cout << endl;
		}
		else
			throw runtime_error("\nException! Resource allocation failed!\n");
		
		syntax.createMIPSFile(outFileName);

		cout << endl << "Output file created." << endl;;

	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
