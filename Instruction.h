#pragma once

/**/
/*
Instruction Class
NAME
	 Instruction - deal with individual instructions from the source code.
DESCRIPTION
	 Instruction class - class to parse and provide information about instructions.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/


class Instruction {

public:

	Instruction() { };
	~Instruction() { };

	// Codes to indicate the type of instruction we are processing.
	enum InstructionType {
		ST_MachineLanguage, // A machine language instruction.
		ST_AssemblerInstr,  // Assembler Language instruction.
		ST_Comment,          // Comment or blank line
		ST_End                    // end instruction.
	};

	// Parse the Instruction.
	InstructionType ParseInstruction(string &a_buff);

	// Translate the Instruction.
	pair<int, string> TranslateInstruction(string &a_buff, int a_loc);

	// Compute the location of the next instruction.
	int LocationNextInstruction(int a_loc);

	// To access the label
	inline string &GetLabel() {

		return m_Label;
	};
	// To determine if a label is blank.
	inline bool isLabel() {

		return !m_Label.empty();
	};
	string set_register(string m_operand);
	string set_operand(string m_operand);

private:
	// Convert a string to lower case.
	string to_lower(string &a_buff);

	// Check for and return the opeartion code of the given string. Returns -1 if it does not.
	int opcode(string &a_buff);

	// adjust a string to a_size digits by adding 0s in the beginning
	string reshape(string &a_buff, int a_size);

	// The elemements of a instruction
	string m_Label;        // The label.
	string m_OpCode;       // The symbolic op code.
	string m_Operand;      // The operand.
	string m_Register;     // The register value.
	bool m_IsNumericOperand;// == true if the operand is numeric.

	string m_instruction;  // The original instruction.

	InstructionType m_type; // The type of instruction.

	vector<string> m_parsed_inst; // Store the parsed instruction
};
