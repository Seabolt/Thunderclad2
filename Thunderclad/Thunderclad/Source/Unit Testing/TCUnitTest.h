//
// TCUnitTest.h
// This file will define the root for all unit testing.
//

#ifndef __TC_UNIT_TEST_H__
#define __TC_UNIT_TEST_H__

//
// Includes
//

#include "TCString.h"

//
// Defines
//

class TCUnitTest
{
	public:		// Memberu
		enum Result
		{
			TestResult_InvalidState = -2,
			TestResult_Failed = -1,
			TestResult_Success = 0,
			TestResult_Success_Skipped,
			TestResult_Running,
		};

	public:		// Methods
							TCUnitTest();
		virtual				~TCUnitTest();
		virtual void		Initialize();
		virtual Result		StartTest();
		virtual Result		FinishTest();
		virtual void		Destroy();
			
		virtual TCString	GetName()		{ return "Invalid Name"; }
		virtual TCString	GetCategory()	{ return "Invalid Category"; }
		virtual float		WaitTime()		{ return 0.0f; }

	private:	// Members
	private:	// Methods
};

#endif // __TC_UNIT_TEST_H__