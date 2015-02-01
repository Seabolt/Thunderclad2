//
// TCUnitTestManager.h
// This class will manage all individual unit tests and will manage their lifecycles.
//

#ifndef __TC_UNIT_TEST_MANAGER_H__
#define __TC_UNIT_TEST_MANAGER_H__

//
// Includes
//

#include "TCList.h"
#include "TCUnitTest.h"

//
// Defines
//

//
// Class Declaration
//

class TCUnitTestManager
{
	public:		// Members
	public:		// Methods
		void Initialize();
		void StartTests();
		void Update();
		void Destroy();
		void AddUnitTest( TCUnitTest* test );

		static TCUnitTestManager* GetInstance()
		{
			static TCUnitTestManager manager;
			return &manager;
		}

	private:	// Members
		TCList< TCUnitTest* > mUnitTests;

		TCUnitTestManager();
		TCUnitTestManager( const TCUnitTestManager& manager );
		TCUnitTestManager& operator=( const TCUnitTestManager& manager );

	private:	// Methods

		void RegisterTests();
};

#endif //__TC_UNIT_TEST_MANAGER_H__