//
// TCLogger.h
// This file will define a global singleton that will track all debug output from this application.
//

#ifndef __TCLOGGER_H__
#define __TCLOGGER_H__

//
// Includes
//

#include "TCString.h"

//
// Defines
//

#define gLogger (TCLogger::GetInstance())

//
// Class Declaration
//

class TCLogger
{
	public:		// Members
		enum LogType
		{
			LOG_INFO,
			LOG_WARNING,
			LOG_ERROR,
			LOG_FAILURE,
		};

	public:		// Methods

		// Singleton, lazy instantiation.
		static inline TCLogger* GetInstance()
		{
			static TCLogger gLoggerInstance;
			return &gLoggerInstance;
		}

		void SetLogName( TCString logName );
		void LogInfo( TCString message );
		void LogWarning( TCString message );
		void LogError( TCString message );
		void LogFailure( TCString message );

	private:	// Members
		TCString mApplicationName;

	private:	// Methods
		TCLogger();
		TCLogger( const TCLogger& logger );
		TCLogger& operator=(const TCLogger& logger);

		void LogString( LogType, TCString& output );
};

#endif // __TCLOGGER_H__