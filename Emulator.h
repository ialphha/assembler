#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

/*
Emulator Class
NAME
	 Emulator - run the Quack3200 machine code generated by the Assembler.
DESCRIPTION
	 Emulator class - operates the emulation of Quack3200 programs
AUTHOR
	 Ishit Lal Pradhan
DATE
	 05/13/2020
*/


class emulator {

public:

	const static int MEMSZ = 100000;	// The size of the memory of the  Quack3200.
	emulator() {

		memset(m_memory, 0, MEMSZ * sizeof(int));
		memset(m_reg, 0, 10 * sizeof(int));

	}

	// Records instructions and data into Quack3200 memory.
	bool insertMemory(int a_location, int a_contents);


	// Runs the quack3200 program recorded in memory.
	bool runProgram();
private:

	int m_memory[MEMSZ];           // The memory of the Quack3200.
	int m_reg[10];                 // The accumulator for the Quack3200

	int m_opcode;                  // Store the opcode for the current statement
	int m_operand;                 // Store the operand for the current statement


	// Functions for read operation in Quack3200 
	void read(int a);
	bool kill = false;  // kill switch
};

#endif
