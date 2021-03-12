#ifndef __IR__
#define __IR__

#include "Types.h"


/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign) {}
	Variable(std::string name, int pos, VariableType type, int val = 0) :
		m_type(type), m_name(name), m_position(pos), m_assignment(no_assign), m_value(val) {}

	int getValue() { return m_value; }
	VariableType getType() { return m_type; }
	std::string getName() { return m_name; }
	int getPosition() { return m_position; }
	Regs getAssignment() { return m_assignment; }
	void Variable::setAssignment(Regs r)
	{
		m_assignment = r;
	}


private:
	int m_value;
	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;
};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;


/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src) {}
	Instruction(int pos, InstructionType type, Variables& dst, Variables& src,
		 std::string& ln, const std::string& currentLabel) :
		m_type(type), m_dst(dst), m_src(src), labelName(ln), label(currentLabel), m_position(pos)
	{
		initializeDefAndUse();
	}

	void initializeDefAndUse();
	void printInstruction();
	int m_position;
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::vector<Instruction*> m_succ;
	std::vector<Instruction*> m_pred;
	std::string labelName;
	std::string label;


	friend std::ostream& operator<<(std::ostream& out, Instruction& i);
};


/* This class represents instruction that contains number token.
Primary use for lw and sw instructions. */
class NumInstruction : public Instruction
{
private:
	int numValue;

public:
	NumInstruction(int pos, InstructionType type, Variables& dst, Variables& src,
		int num, std::string& labelName, std::string& currentLabel)
		: Instruction(pos, type, dst, src, labelName, currentLabel), numValue(num) {}

	/* Getters */
	int GetNumValue() { return numValue; };
};

/**
 * This type represents list of instructions from program code.
 */
typedef std::vector<Instruction*> Instructions;

void printVariables(const Variables& vars);

void printInstructions(Instructions& instructions);

#endif
