#include "stdafx.h"
#include "Errors.h"

static vector<string>m_ErrorMsgs;
/*
Errors::InitErrorReporting()
NAME
	Errors::InitErrorReporting - initializer for the class.
SYNOPSIS
	void Errors::InitErrorReporting();
DESCRIPTION
	Initialize the class for error reporting. The vector is cleared so that it stores all the errors.
RETURNS
AUTHOR
	Ishit Lal Pradhan
DATE
    05/13/2020
*/
void Errors::InitErrorReporting() {
	m_ErrorMsgs.clear();
}

/*
Errors::RecordError(string & a_emsg)
NAME
	Errors::RecordError - records the error encountered.
SYNOPSIS
	void Errors::RecordError(string & a_emsg);
	a_emsg        -->  this parameter is the error in the form of a string which is recorded by the Errors class.
DESCRIPTION
	This function records errors into a vector. It is used to keep the log of the errors occured.
RETURNS
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
void Errors::RecordError(string & a_emsg)
{
	m_ErrorMsgs.push_back(a_emsg);
}
/**/
/*
Errors::DisplayErrors()
NAME
	Errors::DisplayErrors - display the recorded errors.
SYNOPSIS
	void Errors::DisplayErrors();
DESCRIPTION
	Print out all the recorded errors with the line number.
RETURNS
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
void Errors::DisplayErrors()
{
	int count = 0;
	for (vector<string>::iterator it = m_ErrorMsgs.begin(); it != m_ErrorMsgs.end(); ++it) {
		cout << "Error!  " << count++ << "  " << *it << endl;
	}
}
/*
bool Errors::Empty()
NAME
	Errors::Empty - check if empty.
SYNOPSIS
	bool Errors::Empty();
DESCRIPTION
	Check if any errors have been recorded. 
RETURNS
	'true' if no errors have been reported,
	'false' if erros were recorded.
AUTHOR
	Ishit Lal Pradhan
DATE
	05/13/2020
*/
bool Errors::isEmpty() {
	return m_ErrorMsgs.empty();
}