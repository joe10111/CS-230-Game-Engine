//------------------------------------------------------------------------------
//
// File Name:	Trace.c
// Author(s):	Doug Schilling (dschilling), , Joe Centeno(joe.centeno)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <stdarg.h>

#include "AEEngine.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const char* traceFileName = "Trace.log";

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// TODO: Declare a private variable for storing a file handle.
    static FILE * file_pointer;

	static errno_t fileError;
	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------
   
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

// Initialize the Tracing/Logging module.
// - Open "trace.log" for writing in text mode.
// - Perform ERROR handling in the event that the file failed to open.
void TraceInit()
{
	// fopen_s:
	// https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k(stdio%2Ffopen_s);k(fopen_s);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true

	// Error handling (implementation details to be determined by the student):
	// https://msdn.microsoft.com/en-us/library/9t0e6085.aspx
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strerror-s-strerror-s-wcserror-s-wcserror-s?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev16.query%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(STRING%2Fstrerror_s);k(strerror_s);k(DevLang-C%2B%2B);k(TargetOS-Windows)%26rd%3Dtrue&view=vs-2019
	
	//Setting static var to open file 
	fileError = fopen_s(&file_pointer, "trace.log", "wt");
	
	//Checking if log is open  
	if (fileError != 0)
	{
		AESysPrintf("ERROR CANNOT OPEN trace.log");
	}
}

// Output a message to the Tracing/Logging file.
// - Print the given message to the file if it was opened successfully.
// - Every message must be printed on its own line.
// - There must be no blank lines between messages.
void TraceMessage(const char * formatString, ...)
{
	va_list argument_pointer;
	
	if (formatString)
	{
		va_start(argument_pointer, formatString);
		vfprintf_s(file_pointer, formatString, argument_pointer);
		fprintf_s(file_pointer, "\n");
		va_end(argument_pointer);
	}
}

// Shutdown the Tracing/Logging module.
// - Close the file if-and-only-if the file was opened successfully
void TraceShutdown()
{
	if (file_pointer != NULL)
	{
		fclose(file_pointer);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

