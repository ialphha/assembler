#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"



/*
emulator::insertMemory(int a_location, int a_contents)
NAME
	emulator::insertMemory - insert instruction into emulator memory.
SYNOPSIS
	bool emulator::insertMemory(int a_location, int a_contents);
	a_location     --> location of the instruction coming in.
	a_contents     --> content of the instruction coming in.
DESCRIPTION
	Insert machine code instruction with its respective location into the memory of the emulator.
	This function also establishes the location of the first instruction so emulation can be done.
RETURNS
	'true' if the instruction was successfully inserted into memory of the emulator,
	'false' otherwise.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/

bool emulator::insertMemory(int a_location, int a_contents)
{
	if (a_location < MEMSZ && a_location >= 0) {
		m_memory[a_location] = a_contents;
	}
	else {
		string error = "Location out of bounds error";
		Errors::RecordError(error);
		return false;
	}

	return true;
} /* bool emulator::insertMemory(int a_location, int a_contents) */


/**/
/*
emulator::runProgram()
NAME
	emulator::runProgram -run the emulator.
SYNOPSIS
	bool emulator::runProgram();
DESCRIPTION
	Run the emulator on the code stored in the emulator's memory. Instruction is parsed one line
	at a time into op-code and operand and the respective function is called for the op-code.
RETURNS
	'true' if the emulator was successfully run,
	'false' otherwise.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/
bool emulator::runProgram()
{
	int loc = 100;
	for (int i = 0; i < MEMSZ; i++) {
		int contents = m_memory[loc];  // 01455555
		int opcode = contents / 1000000;
		int reg = contents / 100000 % 10; // 14
		int address = contents % 100000;

		switch (opcode)
		{
		case 1:
			m_reg[reg] += m_memory[address];
			loc += 1;
			break;
		case 2:
			m_reg[reg] -= m_memory[address];
			loc += 1;
			break;
		case 3:
			m_reg[reg] *= m_memory[address];
			loc += 1;
			break;
		case 4:
			m_reg[reg] /= m_memory[address];
			loc += 1;
			break;
		case 5:
			m_reg[reg] = m_memory[address];
			loc += 1;
			break;
		case 6:
			m_memory[address] = m_reg[reg];
			loc += 1;
			break;
		case 7:
			read(address);
			loc += 1;
			break;
		case 8:
			cout << m_memory[address] << endl;
			loc += 1;
			break;
		case 9:
			loc = address;
			break;
		case 10:
			if (m_reg[reg] < 0) {
				loc = address;
			}
			else {
				loc += 1;
			}
			break;
		case 11:
			if (m_reg[reg] == 0) {
				loc = address;
			}
			else {
				loc += 1;
			}
			break;
		case 12:
			if (m_reg[reg] > 0) {
				loc = address;
			}
			else {
				loc += 1;
			}
			break;
		case 13:
			kill = true;
			break;
		default:
			cerr << "Illegal opcode" << endl;
			exit(1);
		}
		if (kill) {
			return true;
		}
	}
	return false;
}/* bool emulator::runProgram() */





/*
emulator::read()
NAME
	emulator::read - read function for emulator.
SYNOPSIS
	void emulator::read();
DESCRIPTION
	Read a line from the console and place it in the specified address.
RETURNS
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/

void emulator::read(int address)
{
	string input;
	cout << "?";
	cin >> input;

	char sign = ' ';
	if (input[0] == '-' || input[0] == '+') {
		sign = input[0];
		input = input.erase(0, 1);
	}

	for (unsigned int i = 0; i < input.size(); i++) {
		if (!isdigit(input[i])) {
			cout << "Input is not all digits\n";
			return;
		}
	}

	m_memory[address] = stoi(input);
	if (sign == '-')
		m_memory[address] *= -1;
} /* void emulator::read() */