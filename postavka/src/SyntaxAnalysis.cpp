#include <iostream>
#include <iomanip>
#include <iterator>
#include "SyntaxAnalysis.h"

using namespace std;


SyntaxAnalysis::SyntaxAnalysis(Instructions& ins, LexicalAnalysis& lex, FuncManager& f, LblManager& l, VarManager& v)
	: instructions(ins), lexicalAnalysis(lex), errorFound(false), tokenIterator(lexicalAnalysis.getTokenList().begin()), funcMng(f), lblMng(l), varMng(v), instructionCounter(0)
{
}

void SyntaxAnalysis::createInstruction(InstructionType type, vector<Token>& dst, vector<Token>& src)
{
	Variable* var;
	string labelName = "";
	Instruction* instr;
	Variables* destVars = new Variables;
	Variables* srcVars = new Variables;

	if (type == I_ADD || type == I_SUB || type == I_AND || type == I_OR)
	{
		if ((var = varMng.chackRegVar(dst[0].getValue())) == NULL)
			throw runtime_error("Register variable " + dst[0].getValue() + " is not defined.");

		destVars->push_back(var);
		destVars->front();

		if ((var = varMng.chackRegVar(src[0].getValue())) == NULL)
			throw runtime_error("Register variable " + src[0].getValue() + " is not defined.");

		srcVars->push_back(var);
		srcVars->front();

		if ((var = varMng.chackRegVar(src[1].getValue())) == NULL)
			throw runtime_error("Register variable " + src[1].getValue() + " is not defined.");

		srcVars->push_back(var);
		srcVars->front();

		instr = new Instruction(instructionCounter, type, *destVars, *srcVars,
			labelName, lblMng.currentLabel);
	}
	else if (type == I_ADDI)
	{
		if ((var = varMng.chackRegVar(dst[0].getValue())) == NULL)
			throw runtime_error("Register variable " + dst[0].getValue() + " is not defined.");

		destVars->push_back(var);
		destVars->front();

		if ((var = varMng.chackRegVar(src[0].getValue())) == NULL)
			throw runtime_error("Register variable " + src[0].getValue() + " is not defined.");

		srcVars->push_back(varMng.chackRegVar(src[0].getValue()));
		srcVars->front();

		instr = new NumInstruction(instructionCounter, type, *destVars, *srcVars,
			stoi(src[1].getValue()), labelName, lblMng.currentLabel);
	}
	else if (type == I_LA)
	{
		if ((var = varMng.chackRegVar(dst[0].getValue())) == NULL)
			throw runtime_error("Register variable " + dst[0].getValue() + " is not defined.");

		destVars->push_back(var);
		destVars->front();

		if ((var = varMng.chackMemVar(src[0].getValue())) == NULL)
			throw runtime_error("Memory variable " + src[0].getValue() + " is not defined.");

		srcVars->push_back(var);
		srcVars->front();
		instr = new Instruction(instructionCounter, type, *destVars, *srcVars,
			labelName, lblMng.currentLabel);
	}
	
	else if (type == I_LW || type == I_SW || type == I_LB)
	{
		if ((var = varMng.chackRegVar(dst[0].getValue())) == NULL)
			throw runtime_error("Register variable " + dst[0].getValue() + " is not defined.");

		destVars->push_back(var);
		destVars->front();

		if ((var = varMng.chackRegVar(src[1].getValue())) == NULL)
			throw runtime_error("Register variable " + src[1].getValue() + " is not defined.");

		srcVars->push_back(var);
		srcVars->front();
		instr = new NumInstruction(instructionCounter, type, *destVars, *srcVars,
			stoi(src[0].getValue()), labelName, lblMng.currentLabel);
	}
	else if (type == I_LI)
	{
		if ((var = varMng.chackRegVar(dst[0].getValue())) == NULL)
			throw runtime_error("Register variable " + dst[0].getValue() + " is not defined.");

		destVars->push_back(var);
		destVars->front();
		instr = new NumInstruction(instructionCounter, type, *destVars, *srcVars,
			stoi(src[0].getValue()), labelName, lblMng.currentLabel);
	}
	else if (type == I_BLTZ)
	{
		if ((var = varMng.chackRegVar(src[0].getValue())) == NULL)
			throw runtime_error("Register variable " + src[0].getValue() + " is not defined.");

		srcVars->push_back(var);
		srcVars->front();

		labelName = dst[0].getValue();
		if (!lblMng.checkLabel(labelName))
			throw runtime_error("Label " + labelName + " is not defined.");

		instr = new Instruction(instructionCounter, type, *destVars, *srcVars,
			labelName, lblMng.currentLabel);
	}
	else if (type == I_B)
	{
		labelName = dst[0].getValue();
		if (!lblMng.checkLabel(labelName))
			throw runtime_error("Label " + labelName + " is not defined.");

		instr = new Instruction(instructionCounter, type, *destVars, *srcVars,
			labelName, lblMng.currentLabel);
	}
	else if (type == I_NOP)
	{
		instr = new Instruction(instructionCounter, type, *destVars, *srcVars,
			labelName, lblMng.currentLabel);
	}

	instructions.push_back(instr);
	++instructionCounter;
}

/* Fills list of successors for every instruction.
Starts from the beginning and goes to n - 1.
Also adds predecessors for BRANCH instructions.*/
void SyntaxAnalysis::fillSuccesors() {
	for (int i = 0; i < instructions.size() - 1; i++)
	{
		// Initialize iterator.
		Instructions::iterator it = std::next(instructions.begin(), i);
		// Instruction that is not branch has only 1 successor, the next instruction.
		if ((*it)->m_type != I_B)
			(*it)->m_succ.push_back(*(std::next(instructions.begin(), i + 1)));

		// If instruction is BRANCH instructionType then it has two successors.
		if ((*it)->m_type == I_B || (*it)->m_type == I_BLTZ)
		{
			// Get the position of the succeeding instruction.
			auto instrToJumpIt = instructions.begin();
			std::advance(instrToJumpIt, lblMng.getPosFromLabel((*it)->labelName));
			Instruction* instrToJump = *instrToJumpIt;
			(*it)->m_succ.push_back(instrToJump);

			// Add predecessor for instruction we jump to.
			instrToJump->m_pred.push_back(*it);
		}
	}
}

/* Fills list of predcessors for every instruction.
Starts from the end ang goes to second instruction (n-1). 
*/
void SyntaxAnalysis::fillPredecessors() {
	Instructions::reverse_iterator it;
	for (auto it = instructions.rbegin(); it != instructions.rend() - 1; it++)
	{
		// Add predcessor to the instruction.
		// Predcessor instruction is the one before it.
		if ((*it + 1)->m_type != I_B)
			(*it)->m_pred.push_back(*(it + 1));
	}
}

bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();

	Q();

	return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


void SyntaxAnalysis::printTokenInfo(Token token)
{
	cout << setw(20) << left << token.tokenTypeToString();
	cout << setw(25) << right << token.getValue() << endl;
}


void SyntaxAnalysis::eat(TokenType t)
{
	if (errorFound == false)
	{
		if (currentToken.getType() == t)
		{
			cout << currentToken.getValue() << endl;
			if (currentToken.getType() == T_END_OF_FILE) {
				return;
			}
			currentToken = getNextToken();
		}
		else
		{
			Token expectedToken;
			expectedToken.setType(t);
			errorFound = true;
			throw runtime_error("Invalid token value \"" + currentToken.getValue() + "\" for expected token of type "
				+ expectedToken.tokenTypeToString());
		}
	}
}


Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached.");
	return *tokenIterator++;
}


void SyntaxAnalysis::Q()
{
	if (errorFound == false)
	{
		S();
		eat(T_SEMI_COL);
		L();
	}
}


void SyntaxAnalysis::S()
{
	Token t;
	string name;
	if (errorFound == false)
	{
		if (currentToken.getType() == T_MEM) {
			eat(T_MEM);
			t = currentToken;
			eat(T_M_ID);
			varMng.addMemVar(t, currentToken);
			eat(T_NUM);
		}
		else if (currentToken.getType() == T_REG) {
			eat(T_REG);
			varMng.addRegVar(currentToken);
			eat(T_R_ID);
		}
		else if (currentToken.getType() == T_FUNC) {
			eat(T_FUNC);
			funcMng.addFunc(currentToken, instructionCounter);
			lblMng.addLabel(Label(currentToken.getValue(), instructionCounter));
			eat(T_ID);
		}
		else if (currentToken.getType() == T_ID) {
			name = currentToken.getValue();
			eat(T_ID);
			lblMng.addLabel(Label(name, instructionCounter));
			eat(T_COL);
			E();
		}
		else {
			E();
		}
	}
}


void SyntaxAnalysis::L()
{
	if (errorFound == false)
	{
		if (currentToken.getType() == T_END_OF_FILE) {
			eat(T_END_OF_FILE);
		}
		else {
			Q();
		}
	}
}


void SyntaxAnalysis::E()
{
	vector<Token> src, dst;
	TokenType t;
	if (errorFound == false)
	{
		if (currentToken.getType() == T_ADD || currentToken.getType() == T_AND || currentToken.getType() == T_OR) {
			t = currentToken.getType();
			eat(currentToken.getType());
			dst.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			src.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			src.push_back(currentToken);
			eat(T_R_ID);
			switch (t){
			case T_ADD: createInstruction(I_ADD, dst, src); break;
			case T_AND: createInstruction(I_AND, dst, src); break;
			case T_OR: createInstruction(I_OR, dst, src); break;
			}
		}
		else if (currentToken.getType() == T_ADDI) {
			eat(T_ADDI);
			dst.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			src.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			src.push_back(currentToken);
			eat(T_NUM);
			createInstruction(I_ADDI, dst, src);
		}
		else if (currentToken.getType() == T_SUB) {
			eat(T_SUB);
			dst.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			src.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			src.push_back(currentToken);
			eat(T_R_ID);
			createInstruction(I_SUB, dst, src);
		}
		else if (currentToken.getType() == T_LA) {
			eat(T_LA);
			dst.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			src.push_back(currentToken);
			eat(T_M_ID);
			createInstruction(I_LA, dst, src);
		}
		else if (currentToken.getType() == T_LW || currentToken.getType() == T_LB) {
			t = currentToken.getType();
			eat(t);
			dst.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			src.push_back(currentToken);
			eat(T_NUM);
			eat(T_L_PARENT);
			src.push_back(currentToken);
			eat(T_R_ID);
			eat(T_R_PARENT);
			switch (t) {
			case T_LW: createInstruction(I_LW, dst, src); break;
			case T_LB: createInstruction(I_LB, dst, src); break;
			}
		}
		else if (currentToken.getType() == T_LI) {
			eat(T_LI);
			dst.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			src.push_back(currentToken);
			eat(T_NUM);
			createInstruction(I_LI, dst, src);
		}
		else if (currentToken.getType() == T_SW) {
			eat(T_SW);
			dst.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			src.push_back(currentToken);
			eat(T_NUM);
			eat(T_L_PARENT);
			src.push_back(currentToken);
			eat(T_R_ID);
			eat(T_R_PARENT);
			createInstruction(I_SW, dst, src);
		}
		else if (currentToken.getType() == T_B) {
			eat(T_B);
			dst.push_back(currentToken);
			eat(T_ID);
			createInstruction(I_B, dst, src);
		}
		else if (currentToken.getType() == T_BLTZ) {
			eat(T_BLTZ);
			src.push_back(currentToken);
			eat(T_R_ID);
			eat(T_COMMA);
			dst.push_back(currentToken);
			eat(T_ID);
			createInstruction(I_BLTZ, dst, src);
		}
		else if (currentToken.getType() == T_NOP) {
			eat(T_NOP);
			createInstruction(I_NOP, dst, src);
		}
	}
}

void SyntaxAnalysis::createMIPSFile(string filePath)
{
	ofstream outFile(filePath);
	if (!outFile)
		throw runtime_error("\nException! Failed to open output file!\n");

	outFile << funcMng.getGloblSection();

	outFile << varMng.getDataSection();

	// Fill .text section with instructions and labels
	outFile << "\n.text\n";
	Instructions::iterator it2;

	// Print instructons by labels
	// (Every instructions contains in which label it should be)
	Labels::iterator it = lblMng.labels.begin();
	for (it = lblMng.labels.begin(); it != lblMng.labels.end(); it++)
	{
		outFile << it->name << ":\n";

		// Print instructions for label *it
		for (it2 = instructions.begin(); it2 != instructions.end(); it2++)
		{
			// Exit condition, instruction is in another label
			if (it->name != (*it2)->label)
				continue;
			cout << "\t" << *(*it2) << "\n", outFile << "\t" << *(*it2) << "\n";
		}
	}

	outFile.close();
}

