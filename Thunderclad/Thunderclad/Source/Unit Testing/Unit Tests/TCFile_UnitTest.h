//
// TCFile_UnitTest.h
// This file will define the unit test for TCFile
//

#ifndef __TC_FILE_UNIT_TEST_H__
#define __TC_FILE_UNIT_TEST_H__

//
// Includes
//

#include "TCUnitTest.h"
#include "TCFileManager.h"

//
// Defines
//

//
// Class Declaration
//

class TCFile_UnitTest : public TCUnitTest
{
	public:		// Members
		TCFile_UnitTest( TCFileManager* managerToTest );

	public:		// Methods
		virtual Result StartTest();

	private:	// Members
		TCFileManager* mManagerToTest;

	private:	// Methods
};

#endif // __TC_LIST_UNIT_TEST_H__