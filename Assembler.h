//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"
/*
Assembler Class
NAME
	 Assembler - interpret and translate the input code into Quack3200 machine code.
DESCRIPTION
	 This contains all the components that make up the assembler.
AUTHOR
	 Ishit Lal Pradhan
DATE
	 05/13/2020
*/

class Assembler {

public:
    Assembler( int argc, char *argv[] );
    ~Assembler( );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation
    void PassII( );

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }
    
    // Run emulator on the translation.
    void RunProgramInEmulator();

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab; // Symbol table object
    Instruction m_inst;	    // Instruction object
    emulator m_emul;        // Emulator object
	vector<pair<int, string>> m_machinecode; //vector to store machine code
};

