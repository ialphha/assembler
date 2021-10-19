//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include <vector>
using namespace std;
/*
Errors Class
NAME
	 Errors - hold and display errors encountered during code execution.
DESCRIPTION
	 Emulator class - Class to manage error reporting.
AUTHOR
	 Ishit Lal Pradhan
DATE
	05/13/2020
*/
class Errors {

public:
    
    // Initializes error reports.
	static void InitErrorReporting();

    // Records an error message.
	static void RecordError(string &a_emsg);

    // Displays the collected error message.
	static void DisplayErrors();
	// checks if there's any error retrieved.
	static bool isEmpty();

    
private:
 //vector<string>m_ErrorMsgs;
};
#endif