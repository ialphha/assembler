#include "stdafx.h"
#include <sstream>
using namespace std;
#include "Instruction.h"
#include "Errors.h"
#include "SymTab.h"
/*
Instruction::ParseInstruction(string & a_buff)
NAME
	Instruction::ParseInstruction - parse the incoming instruction.
SYNOPSIS
	Instruction::InstructionType Instruction::ParseInstruction(string & a_buff);
	a_buff    --> this argument is the line from the source code that is to be parsed.
DESCRIPTION
	This function parses the instruction to establish the type of instruction the line being processed is.
	The appropriate instruction type is returned which helps the Pass I function in the Assembler class to
	establish the location of the labels and the origin and end statement. No error is checked for.
RETURNS
	A variable of datatype 'Instruction::InstructionType' which is an enum named 'InstructionType'.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
Instruction::InstructionType Instruction::ParseInstruction(string & a_buff)
{
	// Initializing everything
	m_Label = "";
	m_OpCode = "";
	m_Operand = "";
	m_instruction = a_buff;  // Make copy of original instruction
	m_parsed_inst.clear();
	m_parsed_inst.resize(0);

	// Finding the place where the comment starts in the line
	size_t strt = m_instruction.find(';');

	// Deletes the comment from the line
	if (strt != string::npos)
		m_instruction = m_instruction.substr(0, strt);

	stringstream ss(m_instruction);
	string temp;
	while (ss >> temp)
		m_parsed_inst.push_back(temp);

	// Empty line or line with comment
	if (m_parsed_inst.empty())
		m_type = InstructionType(2); //comment or blank line

   // Instruction to terminate the machine code execution
	else if (to_lower(m_parsed_inst[0]) == "halt")
		m_type = InstructionType(0); //machine language instruction

   // Origin location statement
	else if (to_lower(m_parsed_inst[0]) == "org")
		m_type = InstructionType(1); //assembler instruction

   // Instruction to end the assembly language program
	else if (to_lower(m_parsed_inst[0]) == "end")
		m_type = InstructionType(3); //end instruction

   // The instruction has only two fields : opcode and operand
	else if (m_parsed_inst.size() == 2)
		m_type = InstructionType(0); //assembly language instruction

	else {
		m_Label = m_parsed_inst[0];

		if (m_parsed_inst[1] == "dc" || m_parsed_inst[1] == "DC" || m_parsed_inst[1] == "ds" || m_parsed_inst[1] == "DS")
			m_type = InstructionType(1); //assembler instruction
		else
			m_type = InstructionType(0); //assembly language instruction
	}


	//Check if the operand is number

	return m_type;
} /* Instruction::InstructionType Instruction::ParseInstruction(string & a_buff) */


/**/
/*
Instruction::TranslateInstruction(string & a_buff, int a_loc)
NAME
	Instruction::TranslateInstruction - initializer for the FileAccess class.
SYNOPSIS
	pair<int, string> Instruction::TranslateInstruction(string & a_buff, int a_loc);
	a_buff    --> this argument is the line from the source code that is to be translated.
	a_loc     --> the location of the instruction for the Quack 3200 translated code.
DESCRIPTION
	This function translates instructions in the source code and generates the equivalent machine code for the
	Quak3200 compiler. Function also extensively deals with errors encountered in the source code and reports them
	as necessary.
RETURNS
	a pair with the first element of type int, and the second element of type string.
	the first element is the location of the instruction being translated, and
	the second element is the six digit op-code +register+ operand combo for the emulator.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/
pair<int, string> Instruction::TranslateInstruction(string & a_buff, int a_loc)
{
	// Parse the line and get the instruction type.
	Instruction::InstructionType st = ParseInstruction(a_buff);

	// If the instruction has more that four words, it must be an error
	if (m_parsed_inst.size() > 4) {
		string error = "(location " + to_string(a_loc) + ") More than three field";
		Errors::RecordError(error);
	}

	// For InstructionType(0) -- assembly language instruction which returns the machine code equivalent
	if (st == InstructionType(0)) {
		string assem_code = ""; // To store the assembly language code for the given instruction
		int loc = 0; // To store the location
		switch (m_parsed_inst.size()) {
		case(1):
			m_OpCode = to_lower(m_parsed_inst[0]);
			// Check to see if the word has an OpCode
			if (opcode(m_OpCode) != -1) {
				if (m_OpCode == "halt")
					assem_code = "13900000";
				// Halt is the only OpCode which goes without an operand. Either its "halt" or its an error
				else {
					string error = "(location " + to_string(a_loc) + ") Missing operand";
					Errors::RecordError(error);
					assem_code = to_string(opcode(m_OpCode)) + "??????";
					reshape(assem_code, 8);
				}
			}
			// Report an error otherwise
			else {
				string error = "(location " + to_string(a_loc) + ") Bad Operation Command";
				Errors::RecordError(error);
				assem_code = "????????";
			}
			break;

		case(2):
			m_OpCode = m_parsed_inst[0];
			m_Operand = m_parsed_inst[1];
			m_Register = set_register(m_Operand);
			m_Operand = set_operand(m_Operand);
			if (opcode(m_OpCode) != -1) {
				assem_code = to_string(opcode(m_OpCode));
				reshape(assem_code, 2);

				if (SymbolTable::LookupSymbol(m_Operand, loc)) {
					string a = to_string(loc);
					assem_code = assem_code + m_Register + reshape(a, 5);
				}
				else {
					string error = "(location " + to_string(a_loc) + ") Undefined Operand/Label";
					Errors::RecordError(error);
					assem_code += "????";
				}
			}
			else {
				string error = "(location " + to_string(a_loc) + ") Bad Operation Command";
				Errors::RecordError(error);
				assem_code = "??????";
			}
			break;
		case(3):
			m_Label = m_parsed_inst[0];
			m_OpCode = m_parsed_inst[1];
			m_Operand = m_parsed_inst[2];
			m_Register = set_register(m_Operand);
			m_Operand = set_operand(m_Operand);
			if (opcode(m_OpCode) != -1) {
				assem_code = to_string(opcode(m_OpCode));
				reshape(assem_code, 2);

				if (SymbolTable::LookupSymbol(m_Operand, loc)) {
					string a = to_string(loc);
					assem_code = assem_code + m_Register + reshape(a, 5);
				}
				else {
					string error = "(location " + to_string(a_loc) + ") Undefined Operand/Label";
					Errors::RecordError(error);
					assem_code += "????";
				}
			}
			else {
				string error = "(location " + to_string(a_loc) + ") Bad Operation Command";
				Errors::RecordError(error);
				assem_code = "??????";
			}
			break;
		default:
			assem_code = "??????";
			break;
		}
		cout << setw(12) << left << a_loc << setw(12) << left << assem_code << a_buff << endl;
		return pair<int, string>(a_loc, assem_code);
	}


	if (st == InstructionType(1) && m_parsed_inst.size() >= 3 && (m_parsed_inst[1] == "dc" || m_parsed_inst[1] == "DC"))
		cout << setw(12) << left << a_loc << setw(12) << left << reshape(m_parsed_inst[2], 7) << a_buff << endl;
	// For InstructionType(1) -- assembler instruction
	else if (st == InstructionType(1))
		cout << setw(12) << left << a_loc << setw(12) << left << "" << a_buff << endl;
	// For InstructionType(2) -- comment or blank line and InstructionType(3) -- end
	else
		cout << setw(24) << " " << a_buff << endl;

	// To indicate the end statement
	if (st == InstructionType(3))
		return pair<int, string>(0, "end");

	return pair<int, string>(0, "n/a");
} /* pair<int, string> Instruction::TranslateInstruction(string & a_buff, int a_loc) */

 /*
Instruction::set_register(string m_Operand)
NAME
   Instruction::set_register - sets the register for the operation.
SYNOPSIS
	string Instruction::set_register(string m_Operand) -> sets the string register value.
	m_Operand --> the string to get parsed to dertermine the register value
DESCRIPTION
	This function scans through the operand and returns the string register value.
RETURNS
	Register of the operation.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
string Instruction::set_register(string m_Operand) {
	size_t icomma = m_Operand.find(','); //finding wherer the comma is to seperate the register and the operand
	string Register;
	if (icomma != string::npos)
	{
		Register = m_Operand.substr(0, icomma);
		//m_Operand = m_Operand.substr(icomma + 1);
	}
	else
	{
		Register = "9";
	}
	return Register;
}/*string Instruction::set_register(string m_Operand) */

/*
Instruction::set_operand(string m_Operand)
NAME
   Instruction::set_operand - sets the register for the operation.
SYNOPSIS
	string Instruction::set_operand(string m_Operand) -> sets the string register value.
	m_Operand --> the string to get parsed to determine the value of operand
DESCRIPTION
	This function scans through the operand and returns the string register value.
RETURNS
	Operand value of the operation.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
string Instruction::set_operand(string m_Operand) {
	size_t icomma = m_Operand.find(','); //finding wherer the comma is to seperate the register and the operand
	if (icomma != string::npos)
	{
		//m_Register = m_Operand.substr(0, icomma);
		m_Operand = m_Operand.substr(icomma + 1);
	}

	return m_Operand;
}/*string Instruction::set_operand(string m_Operand) */

/**/
/*
Instruction::LocationNextInstruction(int a_loc)
NAME
	Instruction::LocationNextInstruction - get location for next line of code.
SYNOPSIS
	int Instruction::LocationNextInstruction(int a_loc);
	a_loc   --> location for the current instruction for the Quack3200
DESCRIPTION
	Computes the location for the next line of machine code based on the current instruction.
RETURNS
	Location for the next instruction for the Quack3200
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/
int Instruction::LocationNextInstruction(int a_loc)
{
	// Donot increment location for blank line, comment or end instruction
	if (m_type == InstructionType(2) || m_type == InstructionType(3))
		return a_loc;

	// Check if the current statement is an origin statement
	else if (m_type == InstructionType(1)) {
		stringstream ss(m_instruction);
		string one, two, three;
		ss >> one >> two >> three;

		//returns the origin location stated in the statement
		if (one == "org" || one == "ORG")
			return stoi(two);
		//sets apart storage specified in the statement if it is a define storage statement
		else if (two == "ds" || two == "DS")
			return a_loc + stoi(three);
	}

	// Return one plus the current location to move onto the next instruction
	return a_loc + 1;
} /* int Instruction::LocationNextInstruction(int a_loc) */


/**/
/*
Instruction::to_lower(string &a_buff)
NAME
	Instruction::to_lower - return lowercase of input.
SYNOPSIS
	string Instruction::to_lower(string &a_buff);
	a_buff    --> the string which is to be transformed to lowercase.
DESCRIPTION
	Converts the input string to all lowercase and returns a lowercase copy of the string.
RETURNS
	a lowercase copy of the string passed in as argument.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/
string Instruction::to_lower(string &a_buff)
{
	for (unsigned int i = 0; i < a_buff.size(); i++)
		a_buff[i] = tolower(a_buff[i]);
	return a_buff;
} /* string Instruction::to_lower(string &a_buff) */


/**/
/*
Instruction::opcode(string &a_buff)
NAME
	Instruction::opcode - get opcode for an operation.
SYNOPSIS
	int Instruction::opcode(string &a_buff);
	a_buff --> the operation command to get the op-code of.
DESCRIPTION
	This function returns the respective opcode for an operation.
RETURNS
	an integer between 1 and 13 representing the corresponding opcode,
	-1 if not operation not defined.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/
int Instruction::opcode(string &a_buff)
{
	// Codes to indicate the respective opcode for instructions.
	map<string, int> OpCode{
		 { "add", 1 },{ "sub", 2 },{ "mult", 3 },{ "div", 4 },{ "load", 5 },
		 { "store", 6 },{ "read", 7 },{ "write", 8 },{ "b", 9 },{ "bm", 10 },
		 { "bz", 11 },{ "bp", 12 },{ "halt", 13 }
	};

	if (OpCode[a_buff] <= 13 && OpCode[a_buff] >= 1)
		return OpCode[a_buff];

	return -1;
} /* int Instruction::opcode(string &a_buff) */


/**/
/*
Instruction::reshape(string &a_buff, int a_size)
NAME
	Instruction::reshape- reshape the input string to the a_size.
SYNOPSIS
	string Instruction::reshape(string &a_buff, int a_size);
	a_buff   --> the string to be reshaped.
	a_size   --> the size to which to grow the string to.
DESCRIPTION
	This function reshape a string by adding 0's at the beginning until the desired size is met.
RETURNS
	a reshaped copy of the string passed in as argument.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/
string Instruction::reshape(string &a_buff, int a_size)
{
	if (a_buff.size() == a_size) return a_buff;
	for (int i = a_buff.size(); i < a_size; i++)
		a_buff.insert(0, "0");
	return a_buff;
}