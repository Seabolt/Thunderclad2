//
// TCVertexFormat.h
// This file will define the various vertex formats for a vertex buffer.
//

#ifndef __TC_VERTEX_FORMAT_H__
#define __TC_VERTEX_FORMAT_H__

//
// Includes
//

#include "TCResultCode.h"

//
// Defines
//

#define MAX_VERTEX_ATTRIBUTES 6

//
// TCVertexAttributeType
// This enum will define the various attributes for a vertex buffer.
//

enum TCVertexAttributeType
{
	VertexAttribute_Unknown = -1,
	VertexAttribute_Position,
	VertexAttribute_Color,
	VertexAttribute_Normal,
	VertexAttribute_Binormal,
	VertexAttribute_TexCoord0,
	VertexAttribute_TexCoord1,
	VertexAttribute_TexCoord2,
	VertexAttribute_TexCoord3,
};

//
// TCVertexAttribute
// This class will define the structure of a vertex attribute.
//

class TCVertexAttribute
{
	public:
		TCVertexAttributeType	mType;
		unsigned int			mSize;
		unsigned int			mOffset;
};

//
// This class will encapsulate all the attributes for a vertex.
//

class TCVertexFormat
{
	public:		// Methods

									TCVertexFormat();
									TCVertexFormat( const TCVertexFormat& format );
									TCVertexFormat( TCVertexAttribute* attributes, int numAttributes );
		virtual						~TCVertexFormat();
		virtual	TCVertexFormat&		operator=( const TCVertexFormat& format );

				void				AddAttribute( TCVertexAttributeType attributeType, unsigned int size, unsigned int offset );
				void				AddAttribute( TCVertexAttribute attribute );
				bool				ContainsAttributeType( TCVertexAttributeType attributeType );
				void				GetAttribute( TCVertexAttributeType attributeType, TCVertexAttribute* outAttribute );
		inline	unsigned int		Size()			{ return mVertexSize; }

	public:		// Members
	protected:	// Methods
		virtual void				Clone( const TCVertexFormat& format );

	protected:	// Members
		TCVertexAttribute mAttributes[ MAX_VERTEX_ATTRIBUTES ];
		int				  mNumAttributes;
		unsigned int	  mVertexSize;
};

#endif // __TC_VERTEX_FORMAT_H__