
#pragma once
/*
Symbol Table Class
NAME
	 SymbolTable - this class is our symbol table.
DESCRIPTION
	 SymbolTable class - this class holds the symbol table for the source code.
	 The symbol table is made in the first pass and used in the second pass to
	 determine value for labels and for looking up symbols.
	 Note: LookupSymbol method is static so we can access it anywhere.
AUTHOR
	 Ishit Lal Pradhan
DATE
	 05/13/2020
*/


// This class is our symbol table.
class SymbolTable {

public:
    // Get rid of constructor and destructor if you don't need them.
    SymbolTable( ) {};
    ~SymbolTable( ) {};
    
    const int multiplyDefinedSymbol = -999;

	// Display the symbol table.
	void DisplaySymbolTable();

    // Add a new symbol to the symbol table.
    void AddSymbol( string &a_symbol, int a_loc );

    // Lookup a symbol in the symbol table.
    static bool LookupSymbol(const string &a_symbol, int &a_loc);

private:
  
};
