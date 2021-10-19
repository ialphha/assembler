//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

// This is the actual symbol table.  The symbol is the key to the map.
map<string, int> m_symbolTable;

/*
SymbolTable::AddSymbol(string &a_symbol, int a_loc)
NAME
	SymbolTable::AddSymbol - add a label to the table.
SYNOPSIS
	void SymbolTable::AddSymbol(string &a_symbol, int a_loc);
	a_symbol    --> the symbol that is being added to be the table.
	a_loc       --> the location of the symbol being added.
DESCRIPTION
	This function will place the symbol "a_symbol" and its location "a_loc" into the symbol table.
RETURNS

AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/

void SymbolTable::AddSymbol( string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st= m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}
/*
SymbolTable::DisplaySymbolTable()
NAME
	SymbolTable::DisplaySymbolTable - prints symbol table.
SYNOPSIS
	void SymbolTable::DisplaySymbolTable();
DESCRIPTION
	This function will print out the symbol table.
RETURNS

AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/

void SymbolTable::DisplaySymbolTable()
{
	cout << "Symbol Table:" << endl;
	cout << setw(10) << left << "Symbol #" << setw(10) << left << "Symbol" << setw(10) << left << "Location" << endl;
	int count = 0;
	for (map<string, int>::iterator it = m_symbolTable.begin(); it != m_symbolTable.end(); it++) {
		cout << setw(10) << left << count++ << setw(10) << left << it->first << setw(10) << left << it->second << endl;
	}
	cout << "_______________________________________________________________________________________________________\n\n";
	cout << "Press Enter to continue...\n\n";
	
}/*
SymbolTable::LookupSymbol(const string & a_symbol, int & a_loc)
NAME
    SymbolTable::LookupSymbol - lookup a symbol in the table.
SYNOPSIS
    bool SymbolTable::LookupSymbol(const string & a_symbol, int & a_loc);
    a_symbol    --> the symbol to be searched for. const before this string indicates the function is not going to change the value at the address of this string.
    a_loc       --> if the symbol is found, the location is stored in a_loc. if not, a_loc is unchanged.
DESCRIPTION
    This function will lookup a symbol in the symbol table and if found, store the location of the symbol in a_loc.
RETURNS
    'true' if symbol was found in the table,
    'false' if not found.
AUTHOR
    Ishit Lal Pradhan
DATE
    05/13/2020
*/

bool SymbolTable::LookupSymbol(const string &a_symbol, int &a_loc) {
	map<string, int>::iterator it = m_symbolTable.find(a_symbol);
	if (it == m_symbolTable.end()) return false;
	a_loc = it->second;
	return true;
}
