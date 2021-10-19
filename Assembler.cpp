//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

/**/
/*
Assembler::Assembler( int argc, char *argv[] )
NAME
	Assembler::Assembler - constructor for the Assembler class.
SYNOPSIS
	Assembler::Assembler( int argc, char *argv[] );
	argc              --> total number of arguments received in the command line.
	*argv[]           --> a pointer to the array of arguments passed through the command line.
DESCRIPTION
	Constructor for the assembler. Note: we are passing argc and argv to the file access constructor.
RETURNS
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/
Assembler::Assembler(int argc, char *argv[])
	: m_facc(argc, argv)
{

	// Nothing else to do here at this point.

} /* Assembler::Assembler( int argc, char *argv[] ) */

// Destructor currently does nothing.  You might need to add something as you develope this project.
Assembler::~Assembler()
{
}
// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.

/**/
/*
Assembler::PassI()
NAME
	Assembler::PassI - first pass through the source code.
SYNOPSIS
	Assembler::PassI();
DESCRIPTION
	This function is the first pass for the Assembler. Pass I establishes the location of the
	labels and constructs a symbol table using the SymbolTable class.
RETURNS
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/
void Assembler::PassI()
{
	int loc = 0;        // Tracks the location of the instructions to be generated.

	// Successively process each line of source code.
	while (true) {

		// Read the next line from the source file.
		string line;
		if (!m_facc.GetNextLine(line)) {

			// We will let this error be reported by Pass II.
			return;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st=m_inst.ParseInstruction(line);

		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) return;

		// Labels can only be on machine language and assembler language
		// instructions.  So, skip other instruction types.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr)
		{
			continue;
		}
		// If the instruction has a label, record it and its location in the
		// symbol table.
		if (m_inst.isLabel()) {

			m_symtab.AddSymbol(m_inst.GetLabel(), loc);
		}
		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
} /* void Assembler::PassI() */

/**/
/*
Assembler::PassII()
NAME
	Assembler::PassII -> second pass through the source code.
SYNOPSIS
	void Assembler::PassII();
DESCRIPTION
	PassII does most of the work in this program. Pass II does a second traversal of the source code.
	It uses the symbol table established in Pass I and starts translating the source code into code
	for the Quack3200 computer. It records any errors encountered, and stores the translated instruction
	in a vector of pairs for further use. Pass II also prints out the original
	statement and the translated code for every line of instruction in the source code with help from the
	TranslateInstruction function in the Instruction class. In the end, if errors have been encountered,
	the function prints them out.
RETURNS
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/

void Assembler::PassII() {
	m_facc.rewind();  //
	int loc = 0;
	bool is_end = false;
	Errors::InitErrorReporting();
	m_machinecode.clear();
	cout << "Translation of Program:" << endl;
	cout << setw(10) << left << "Location" << setw(10) << left << "Contents" << "Original Statement" << endl;

	// Successively process each line of source code.
	for (; ; ) {
		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {

			// Returned as was expected with no lines after code
			if (is_end == true)
				break;

			// Report error : since there are no more lines, we are missing an end statement
			string error = "(location " + to_string(loc) + ") Missing end statement";
			Errors::RecordError(error);
			break;
		}

		if (is_end == true) {
			string error = "(location " + to_string(loc) + ") Lines after end statement";
			Errors::RecordError(error);
			break;
		}

		pair<int, string>  translation = m_inst.TranslateInstruction(buff, loc);

		// Set the is_end flag to true to indicate the apperance of end statement
		if (translation == pair<int, string>(0, "end"))
			is_end = true;
		// Do not push the pair onto the vector if there is no valid machine code
		else if (translation != pair<int, string>(0, "n/a"))
			m_machinecode.push_back(translation);

		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
	if (!Errors::isEmpty())
		Errors::DisplayErrors();
	cout << "__________________________________________________________________________________________________\n\n";
	cout << "Press Enter to continue...\n\n";
}
/*
Assembler::RunEmulator()
NAME
	Assembler::RunEmulator - run the emulator on the translated code.
SYNOPSIS
	void Assembler::RunEmulator();
DESCRIPTION
	Run the emulator on the translated code from Pass II. THe emulation is halted if any errors have been encountered.
	This function loads all the translated instruction into the emulator's memory and then runs the emulator if there's
	no error. It also  detects errors encountered during emulation and reports them after.
RETURNS
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
/**/
void Assembler::RunProgramInEmulator() {
	if (!Errors::isEmpty()) {
		cout << "Errors were encountered " << endl;
		cout << "Exiting emulation" << endl;
		return;
	}
	cout << "Results from emulating program:\n\n";
	for (vector<pair<int, string>>::iterator it = m_machinecode.begin(); it != m_machinecode.end(); ++it) {
		bool insertion_check = m_emul.insertMemory(it->first, stoi(it->second));
		if (insertion_check == false) {
			string error = "Error inserting the command " + to_string(it->first) + " " + it->second + " into the emulator memory";
			Errors::RecordError(error);
		}
	}
	bool run_check = m_emul.runProgram();
	if (run_check == false) {
		string error = "Error running the emulator";
		Errors::RecordError(error);
	}
	// Display any errors encountered during emulation.
	if (!Errors::isEmpty())
		Errors::DisplayErrors();

	cout << "\nEnd of emulation";
}