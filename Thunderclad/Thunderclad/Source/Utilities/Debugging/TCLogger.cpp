//
// TCLogger.cpp
// This file will define logging functionality.
//

//
// Includes
//

#include "TCLogger.h"
#include <Windows.h>

//
// Defines
//

//
// Default Constructor
//		- Will initialize this object to a safe state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCLogger::TCLogger()
{
	mApplicationName = "Unnamed Thunderclad App";
}

//
// Copy Constructor
//		- Will create a new instance of a logger as a copy of another logger.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCLogger::TCLogger( const TCLogger& logger )
{
	mApplicationName = logger.mApplicationName;
}

//
// Assignment Operator
//		- Will set this logger to another logger
// Inputs:
//		- const TCLogger& logger.
// Outputs:
//		- TCLogger& logger.
//

TCLogger& TCLogger::operator=( const TCLogger& logger )
{
	mApplicationName = logger.mApplicationName;
	return *this;
}

//
// SetLogName
//		- Will set the application name for the logger.
// Inputs:
//		- TCString& name: the name of the application.
// Outputs:
//		- None.
//

void TCLogger::SetLogName( TCString name )
{
	mApplicationName = name;
}

//
// LogInfo
//		- Will log information about the app, this is the lowest priority.
// Inputs:
//		- TCString& string: The log.
// Outputs:
//		- None.
//

void TCLogger::LogInfo( TCString log )
{
	TCString output = "[";
	output += mApplicationName;
	output += " - INFO]\t\t";
	output += log;
	output += '\n';

	LogString( LOG_INFO, output );
}

//
// LogWarning
//		- Will log a warning for the app, this is something that is not gamebreaking, but still something that should be known.
// Inputs:
//		- TCString& string: The message/
// Outputs:
//		- None.
//

void TCLogger::LogWarning( TCString log )
{
	TCString output = "[";
	output += mApplicationName;
	output += " - WARNING]\t\t";
	output += log;
	output += '\n';

	LogString( LOG_WARNING, output );
}

//
// LogError
//		- Will log an error for the app, this is something that is potentially gamebreaking
// Inputs:
//		- TCString& string: The message.
// Outputs:
//		- None.
//

void TCLogger::LogError( TCString log )
{
	TCString output = "[";
	output += mApplicationName;
	output += " - ERROR]\t\t";
	output += log;
	output += '\n';

	LogString( LOG_ERROR, output );
}

//
// LogFailure
//		- Will log a failure for the app, the game is broken.
// Inputs:
//		- TCString& string: The message.
// Outputs:
//		- None.
//

void TCLogger::LogFailure( TCString log )
{
	TCString output = "[";
	output += mApplicationName;
	output += " - FAILURE]\t\t";
	output += log;
	output += '\n';

	LogString( LOG_FAILURE, output );
}

//
// LogString
//		- Will determine which file to output to and flush the output.
// Inputs:
//		- TCString& message: The message for the log.
// Outputs:
//		- None.
//

void TCLogger::LogString( TCLogger::LogType type, TCString& log )
{
	OutputDebugString( log.Data() );
}