#include "IR.h"
#include <iostream>

using namespace std;
void Instruction::initializeDefAndUse()
{
	// Initialize def and use sets when an instruction is created.
	switch (m_type)
	{
	case I_ADD:  // add rid,rid,rid
	case I_SUB:  // sub rid,rid,rid
		m_def.insert(m_def.end(), m_dst.begin(), m_dst.end());
		m_use.insert(m_use.end(), m_src.begin(), m_src.end());
		break;

	case I_ADDI: // addi rid,rid,num
		m_def.insert(m_def.end(), m_dst.begin(), m_dst.end());
		m_use.insert(m_use.end(), m_src.begin(), m_src.end());
		break;

	case I_BLTZ: // bltz rid,id
		m_use.insert(m_use.end(), m_src.begin(), m_src.end());
		break;

	case I_LA: // la rid,mid
	case I_LI: // li rid,num
		m_def.insert(m_def.end(), m_dst.begin(), m_dst.end());
		break;
	case I_LW: // lw rid,num(rid)
		m_use.insert(m_use.end(), m_src.begin(), m_src.end());
		m_def.insert(m_def.end(), m_dst.begin(), m_dst.end());
		break;
	case I_SW: // sw rid,num(rid)
		m_use.insert(m_use.end(), m_src.begin(), m_src.end());
		break;
	case I_B:   // b id
	case I_NOP: // nop
		break;
	}
}

void printVariables(const Variables& vars)
{
	Variables::const_iterator it = vars.begin();
	while (it != vars.end())
	{
		cout << (*it)->getName() << " ";
		it++;
	}
}


void printInstructions(Instructions& instructions)
{
	Instructions::iterator it = instructions.begin();
	while (it != instructions.end())
	{
		(*it)->printInstruction();
		cout << "-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-" << endl;
		it++;
	}
}

/* Prints instruction with all variable lists. */
void Instruction::printInstruction()
{
	cout << "Instruction " << m_position;
	cout << endl;

	cout << "SRC: ";
	printVariables(m_src);
	cout << endl;

	cout << "DEST: ";
	printVariables(m_dst);
	cout << endl;

	cout << "USE: ";
	printVariables(m_use);
	cout << endl;

	cout << "DEF: ";
	printVariables(m_def);
	cout << endl;

	cout << "IN: ";
	printVariables(m_in);
	cout << endl;

	cout << "OUT: ";
	printVariables(m_out);
	cout << endl;

	cout << endl;
}

ostream& operator<<(ostream& out, Instruction& i)
{
	Variable* v;
	Variables::iterator it;

	switch (i.m_type)
	{
	case I_ADD: // add rid,rid,rid
		out << "add $t" << i.m_dst.front()->getAssignment();
		it = i.m_src.begin();
		while (it != i.m_src.end())
		{
			out << ", $t" << (*it)->getAssignment();
			it++;
		}
		break;
	case I_AND: // add rid,rid,rid
		out << "and $t" << i.m_dst.front()->getAssignment();
		it = i.m_src.begin();
		while (it != i.m_src.end())
		{
			out << ", $t" << (*it)->getAssignment();
			it++;
		}
		break;
	case I_OR: // add rid,rid,rid
		out << "or $t" << i.m_dst.front()->getAssignment();
		it = i.m_src.begin();
		while (it != i.m_src.end())
		{
			out << ", $t" << (*it)->getAssignment();
			it++;
		}
		break;

	case I_SUB: // sub rid,rid,rid
		out << "sub $t" << i.m_dst.front()->getAssignment();
		it = i.m_src.begin();
		while (it != i.m_src.end())
		{
			out << ", $t" << (*it)->getAssignment();
			it++;
		}
		break;


	case I_ADDI: // addi rid,rid,num
		out << "addi $t" << i.m_dst.front()->getAssignment() << ", $t";
		out << i.m_src.front()->getAssignment();
		out << ", ";
		out << (reinterpret_cast<NumInstruction*>(&i))->GetNumValue();
		break;


	case I_BLTZ: // bltz rid,id
		out << "bltz $t" << i.m_src.front()->getAssignment() << ", ";
		out << i.labelName;
		break;

	case I_B:   // b id
		out << "b ";
		out << i.labelName;
		break;

	case I_LI: // li rid,num
		out << "li $t";
		out << i.m_dst.front()->getAssignment() << ", ";
		out << (reinterpret_cast<NumInstruction*>(&i))->GetNumValue();
		break;

	case I_LA: // la rid,mid
		out << "la $t" << i.m_dst.front()->getAssignment();
		out << ", ";
		out << i.m_src.front()->getName();
		break;

	case I_LW: // lw rid,num(rid)
		v = i.m_src.front();
		out << "lw $t" << v->getAssignment();
		out << ", " << (reinterpret_cast<NumInstruction*>(&i))->GetNumValue()
			<< "($t" << v->getAssignment() << ")";
		break;
	case I_LB: // lw rid,num(rid)
		v = i.m_src.front();
		out << "lb $t" << v->getAssignment();
		out << ", " << (reinterpret_cast<NumInstruction*>(&i))->GetNumValue()
			<< "($t" << v->getAssignment() << ")";
		break;

	case I_NOP: // nop
		out << "nop";
		break;

	case I_SW: // sw rid,num(rid)
		out << "sw $t" << i.m_dst.front()->getAssignment();
		out << ", " << (reinterpret_cast<NumInstruction*>(&i))->GetNumValue()
			<< "($t" << i.m_src.front()->getAssignment() << ")";
		break;
	}

	return out;
}
