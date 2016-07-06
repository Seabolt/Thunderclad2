//
// TCShaderAttribute.h
// This file will define the functionality of a shader attribute
//

#ifndef __TC_SHADER_ATTRIBUTE_H__
#define __TC_SHADER_ATTRIBUTE_H__

//
// Includes
//

#include "TCGraphicsContext.h"
#include "IGraphicsResource.h"

//
// Defines
//

//
// Forward Declaration
//

//
// Class Declaration
//

class TCShaderAttribute
	: IGraphicsResource
{
	public:		// Members
		
		enum Usage
		{
			kShaderAttributeUsageUnknown = -1,
			kShaderAttributeUsagePosition,
			kShaderAttributeUsageNormal,
			kShaderAttributeUsageTexCoord0,
			kShaderAttributeUsageTexCoord1,
			kShaderAttributeUsageTexCoord2,
			kShaderAttributeUsageTexCoord3,
			kShaderAttributeUsageColor0,
			kShaderAttributeUsageColor1,
			kShaderAttributeUsageColor2,
			kShaderAttributeUsageBoneIndex,
			kShaderAttributeUsageBoneWeight,
			kNumShaderAttributeUsages
		};

		enum Type
		{
			kShaderAttributeTypeUnknown = -1,
			kShaderAttributeTypeFloat,
			kShaderAttributeTypeFloat2,
			kShaderAttributeTypeFloat3,
			kShaderAttributeTypeFloat4,
			kShaderAttributeTypeMatrix3x3,
			kShaderAttributeTypeMatrix4x4,
			kNumShaderAttributeTypes
		};

		struct Description
		{
			Type		type;
			Usage		usage;
			TCString	name;
		};

	public:		// Methods
							TCShaderAttribute( TCGraphicsContext* context );
							TCShaderAttribute( const TCShaderAttribute& toCopy );
							~TCShaderAttribute();
		TCShaderAttribute&	operator=( const TCShaderAttribute& toCopy );
		
		TCResult			Initialize( Description* description );
		TCResult			Release();

		TCString			GetName()	{ return mName; }
		Type				GetType()	{ return mType; }
		Usage				GetUsage()	{ return mUsage; }

		static Usage		StringToUsage( TCString& string );
		static Type			StringToType( TCString& string );
		static TCString		TypeToString( Type type );
		static TCString		UsageToString( Usage usage );
	protected:	// Members
		TCString							mName;
		Usage								mUsage;
		Type								mType;
		TCGraphicsContext::TCShaderContext* mContext;

	protected:	// Methods

		void				Clone( const TCShaderAttribute& toCopy );
};

#endif // __TC_SHADER_ATTRIBUTE_H__