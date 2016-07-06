//
// TCList.hpp
// This file will define the methods declared for a list.
//

#ifndef __TCLIST_HPP__
#define __TCLIST_HPP__

//
// Includes
//

#include "TCList.h"
#include "TCMemUtils.h"
#include "TCMathUtils.h"

//
// Defines
//

#define DEFAULT_GROWTH_RATE (1.0f)

//
// Default Constructor
//		- Initializes the list to a save state.
// Inputs:
//		- None
// Outputs:
//		- None.
//

template <class Object>
TCList< Object >::TCList()
{
	mGrowthRate = DEFAULT_GROWTH_RATE;
	mNumObjects = 0;
	mList		= NULL;
	mCapacity	= 0;
}

//
// Growth Rate Constructor
//		- Initialize the list to a safe state with a specified growth rate.
// Inputs:
//		- float growthRate: The rate at which the expansion of the list should happen.
// Outputs:
//		- None.
//

template <class Object>
TCList< Object >::TCList( float growthRate )
{
	mGrowthRate = growthRate;
	mNumObjects	= 0;
	mCapacity	= 0;
	mList		= NULL;
}

//
// Copy Constructor
//		- Initializes the list to copy of the list provided.
// Inputs:
//		- const TCList& list: The list to copy.
// Outputs:
//		- None.
//

template <class Object>
TCList< Object >::TCList( const TCList& toCopy )
{
	mList = NULL;
	Copy( toCopy );
}

//
// Destructor
//		- Will free all resources associated with this object
// Inputs:
//		- None.
// Outputs:
//		- None.
//

template <class Object>
TCList< Object >::~TCList()
{
	Clear();
}

//
// Assignment Operator
//		- Will set this list equal to the list provided.
// Inputs:
//		- const TCList& list: The list to copy.
// Outputs:
//		- const TCList& list: The reference to this object.
//

template <class Object>
TCList<Object>& TCList< Object >::operator=( const TCList<Object>& list )
{
	Clear();
	Copy( list );

	return *this;
}

//
// Append
//		- Will add an object to the end of the list.
// Inputs:
//		- Object: The object to add.
// Outputs:
//		- None.
//

template <class Object>
void TCList< Object >::Append( Object object )
{
	//
	// Check to see if we need to increase our capacity.
	//

	if( mNumObjects + 1 > mCapacity )
	{
		IncreaseCapacity();
	}

	//
	// Add the object.
	//

	mList[ mNumObjects ] = object;
	mNumObjects++;
}

//
// Remove
//		- Will remove an object from the list.
// Inputs:
//		- Object& object: The object to remove from the list.
// Ouputs:
//		- None.
//

template <class Object>
void TCList< Object >::Remove( Object object )
{
	int index = Find( object );
	if( index >= 0 )
	{
		RemoveAt( index );
	}
}

//
// RemoveAt
//		- Will remove an object from the list.
// Inputs:
//		- int index: The index to an object to remove from the list.
// Outputs:
//		- None.
//

template <class Object>
void TCList< Object >::RemoveAt( int index )
{
	if( index < 0 || index > mNumObjects )
	{
		TC_ASSERT( 0 && "Tried to remove an object from a list at an invalid index." );
		return;
	}
	
	//
	// Shift the list down one.
	//

	for( int currentObject = index; currentObject < mNumObjects; ++currentObject )
	{
		if( currentObject + 1 < mNumObjects )
		{
			mList[ currentObject ] = mList[ currentObject + 1 ];
		}
	}

	//
	// Update the object size.
	//

	mNumObjects--;
}

//
// Insert
//		- Will add an object at a specific location.
// Inputs:
//		- Object& object: The object to add.
//		- int index: The index to add the object to at.
// Outputs:
//		- None.
//

template < class Object >
void TCList< Object >::Insert( Object object, int index )
{
	if( index < 0 || index >= mNumObjects )
	{
		TC_ASSERT( 0 && "Tried to insert an object into a TCList at an invalid index." );
		return;
	}

	//
	// Make sure we have space.
	//

	if( mNumObjects + 1 > mCapacity )
	{
		IncreaseCapacity();
	}

	//
	// Shift list forward.
	//

	for( int currentObject = mNumObjects - 1; currentObject >= index; --currentObject )
	{
		mList[ currentObject + 1 ] = mList[ currentObject ];		
	}

	//
	// Add the object
	//

	mNumObjects++;
	mList[ index ] = object;
}

//
// Clear
//		- Will remove all entries from the list and release the list resources.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

template <class Object>
void TCList< Object >::Clear()
{
	mCapacity = 0;
	mNumObjects = 0;
	TC_SAFE_DELETE_ARRAY( mList );
}

//
// Contains
//		- Will return whether or not the object specified is in the list.
// Inputs:
//		- Object& object: The object to search for.
// Outputs:
//		- bool: Was the object found or not.
//

template <class Object>
bool TCList< Object >::Contains( Object object )
{
	return ( Find( object ) != -1 );
}

//
// Find
//		- Will return the index of an object provided in the list, or -1 if not found.
// Inputs:
//		- Object& object: The object to search for.
// Outputs:
//		- int: The index of the object, -1 if not found.
//

template <class Object>
int TCList< Object >::Find( Object object )
{
	for( int currentObject = 0; currentObject < mNumObjects; ++currentObject )
	{
		if( mList[ currentObject ] == object )
		{
			return currentObject;
		}
	}

	return -1;	// We didn't find it.
}

//
// Reserve
//		- Will increase the capacity of this object to the specified size.
// Inputs:
//		- int: The new capacity size.
// Outputs:
//		- None.
//

template <class Object>
void TCList< Object >::Reserve( int capacity )
{
	if( mCapacity > capacity )
		return;

	//
	// Increase the size of the list..
	//

	mCapacity = capacity;

	Object* oldList = mList;
	mList = new Object[ mCapacity ];

	//
	// Copy over the old list.
	//

	for( int currentObject = 0; currentObject < mNumObjects; ++currentObject )
	{
		mList[ currentObject ] = oldList[ currentObject ];
	}

	//
	// Free the old list.
	//

	TC_SAFE_DELETE_ARRAY( oldList );
}

//
// Resize
//		- This will increase the capacity to the size provided and will update the count.
// Inputs:
//		- int size: The new size to grow to.
// Outputs:
//		- None.
//

template <class Object>
void TCList< Object >::Resize( int size )
{
	Reserve( size );
	mNumObjects = size;
}

//
// Copy
//		- Will copy over all information from the other list to this list.
// Inputs:
//		- const TCList& list: The list to copy.
// Outputs:
//		- void.
//

template <class Object>
void TCList< Object >::Copy( const TCList<Object>& toCopy )
{
	//
	// Free old memory.
	//

	Clear();
	
	//
	// Copy over variables.
	//

	mCapacity = toCopy.mCapacity;
	mGrowthRate = toCopy.mGrowthRate;
	mNumObjects = toCopy.mNumObjects;

	//
	// Copy over the list information.
	//

	mList = new Object[ mCapacity ];
	for( int currentObject = 0; currentObject < mNumObjects; ++currentObject )
	{
		mList[ currentObject ] = toCopy.mList[ currentObject ];
	}
}

//
// IncreaseCapacity
//		- Will increase the capacity of the list with a specified growth rate.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

template <class Object>
void TCList< Object >::IncreaseCapacity()
{
	//
	// Calculate our new capacity.
	//

	mCapacity = TC_MAX((int)((float)mCapacity * mGrowthRate), mCapacity + 1);
	
	//
	// Allocate a new array.
	//

	Object* oldList = mList;
	mList = new Object[ mCapacity ];

	//
	// Copy over the old list.
	//

	for( int currentObject = 0; currentObject < mNumObjects; ++currentObject )
	{
		mList[ currentObject ] = oldList[ currentObject ];
	}

	//
	// Delete the old list.
	//

	TC_SAFE_DELETE_ARRAY( oldList );
}

#endif // __TCLIST_HPP__