//
// TCVertexFormat.cpp
// This file will implement the functionality for a vertex format.
//

//
// Includes
//

#include "TCVertexFormat.h"
#include "TCPlatformPrecompilerSymbols.h"
#include "TCLogger.h"

//
// Defines
//

//
// TCVertexFormat Default Constructor
//		- This will initialize the format to a safe state.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCVertexFormat::TCVertexFormat()
{
	for( int currentAttribute = 0; currentAttribute < MAX_VERTEX_ATTRIBUTES; ++currentAttribute )
	{
		mAttributes[ currentAttribute ].mType = VertexAttribute_Unknown;
		mAttributes[ currentAttribute ].mSize = 0;
		mAttributes[ currentAttribute ].mOffset = 0;
	}

	mNumAttributes = 0;
}

//
// Copy Constructor
//		- Will copy this object from another vertex format.
// Inputs:
//		- const TCVertexFormat& format: The format to copy.
// Outputs:
//		- None.
//

TCVertexFormat::TCVertexFormat( const TCVertexFormat& format )
{
	Clone( format );
}

//
// Memberwise Initialization Constructor
//		- Will initialize this object to the parameters provided.
// Inputs:
//		- TCVertexAttribute* attribute: The attributes to copy.
// Outputs:
//		- None.
//

TCVertexFormat::TCVertexFormat( TCVertexAttribute* attributes, int numAttributes )
{
	if( attributes == NULL ) return;

	for( int currentAttribute = 0; currentAttribute < numAttributes; ++currentAttribute )
	{
		mAttributes[ currentAttribute ] = attributes[ currentAttribute ];
	}

	mNumAttributes = numAttributes;
}

//
// Assignment Operator
//		- Will set this instance equal to another.
// Inputs:
//		- const TCVectexFormat& format: The format to copy from.
// Outputs:
//		- TCVertexFormat& format: The reference to this after it's been assigned to.
//

TCVertexFormat& TCVertexFormat::operator=( const TCVertexFormat& format )
{
	Clone( format );
	return *this;
}

//
// Destructor
//		- Will release all resources associated with this object upon being destroyed.
// Inputs:
//		- None.
// Outputs:
//		- None.
//

TCVertexFormat::~TCVertexFormat()
{
	
}

//
// AddAttribute
//		- Will add a new attribute to this format.
// Inputs:
//		- TCVertexAttributeType attributeType: The type of vertex attribute.
//		- unsigned int size: The size of the attribute
//		- unsigned int offset: The offset in the vertex buffer.
// Outputs:
//		- None.
//

void TCVertexFormat::AddAttribute( TCVertexAttributeType attributeType, unsigned int size, unsigned int offset )
{
	if( mNumAttributes + 1 > MAX_VERTEX_ATTRIBUTES )
	{
		gLogger->LogError( "[TCVertexFormat] Failed to add attribute! Max attributes reached." );
		return;
	}

	if( attributeType == VertexAttribute_Unknown )
	{
		gLogger->LogError( "[TCVertexFormat] Failed to add attribute! Attribute type is unknown!" );
		return;
	}

	if( size == 0 )
	{
		gLogger->LogError( "[TCVertexFormat] Failed to add attribute! Attribute size is zero!" );
		return;
	}

	mAttributes[ mNumAttributes ].mType		= attributeType;
	mAttributes[ mNumAttributes ].mSize		= size;
	mAttributes[ mNumAttributes ].mOffset	= offset;

	mVertexSize += size;	// Increase the size.
	mNumAttributes++;		// Increment the attribute counter.
}

void TCVertexFormat::AddAttribute( TCVertexAttribute attribute )
{
	AddAttribute( attribute.mType, attribute.mSize, attribute.mOffset );
}

//
// ContainsAttributeType
//		- Will determine if there is an attribute for the specified type.
// Inputs:
//		- TCVertexAttributeType attributeType: The attribute to search for.
// Outputs:
//		- bool: Does this format have the requested attribute.
//

bool TCVertexFormat::ContainsAttributeType( TCVertexAttributeType attributeType )
{
	for( int currentAttribute = 0; currentAttribute < mNumAttributes; ++currentAttribute )
	{
		if( mAttributes[ currentAttribute ].mType == attributeType )
		{
			return true;
		}
	}

	return false;
}

//
// GetAttribute
//		- Will get the attribute for the type provided.
// Inputs:
//		- TCVertexAttributeType attributeType: The attribute to search for.
//		- TCVertexAttribute* attribute: The attribute found.
// Outputs:
//		- TCVertexAttribute: The attribute found.
//

void TCVertexFormat::GetAttribute( TCVertexAttributeType attributeType, TCVertexAttribute* attribute )
{
	if( attribute == NULL ) return;

	for( int currentAttribute = 0; currentAttribute < mNumAttributes; ++currentAttribute )
	{
		if( mAttributes[ currentAttribute ].mType == attributeType )
		{
			*attribute = mAttributes[ currentAttribute ];
		}
	}
}

//
// Clone
//		- Will copy another TCVertexFormat.
// Inputs:
//		- const TCVertexFormat& in: The reference to copy.
// Outputs:
//		- None.
//

void TCVertexFormat::Clone( const TCVertexFormat& in )
{
	mNumAttributes = in.mNumAttributes;

	for( int currentAttribute = 0; currentAttribute < mNumAttributes; ++currentAttribute )
	{
		mAttributes[ currentAttribute ] = in.mAttributes[ currentAttribute ];
	}
}