//
// TCShaderUniform.h
// This class will define the platform agnostic information regarding a shader uniform.
//

#ifndef __TC_SHADER_UNIFORM_H__
#define __TC_SHADER_UNIFORM_H__

//
// Includes
//

#include "TCGraphicsContext.h"
#include "TCString.h"
#include "IGraphicsResource.h"

//
// Defines
//

//
// Forward Declarations
//

class TCConstantBuffer;

//
// Class Declaration
//

class TCShaderUniform
	: IGraphicsResource
{
	public: // Members
		enum Type
		{
			kShaderUniformTypeUnknown = -1,
			kShaderUniformTypeFloat,
			kShaderUniformTypeFloat2,
			kShaderUniformTypeFloat3,
			kShaderUniformTypeFloat4,
			kShaderUniformTypeHalf,
			kShaderUniformTypeHalf2,
			kShaderUniformTypeHalf3,
			kShaderUniformTypeHalf4,
			kShaderUniformTypeInt,
			kShaderUniformTypeInt2,
			kShaderUniformTypeInt3,
			kShaderUniformTypeInt4,
			kShaderUniformTypeBool,
			kShaderUniformTypeBool2,
			kShaderUniformTypeBool3,
			kShaderUniformTypeBool4,
			kShaderUniformTypeMatrix3x3,
			kShaderUniformTypeMatrix4x4,
			kNumShaderUniformTypes,
		};

		enum Usage
		{
			kShaderUniformUsageUnknown,
			kShaderUniformUsageEyePosition,
			kShaderUniformUsageWorldPosition,
			kShaderUniformUsageProperty,
			kNumShaderUniformUsages,
		};

		struct Description
		{
			TCString			name;
			Type				type;
			Usage				usage;
			TCString			constantBufferName;
			int					constantBufferOffset;
			int					globalRegister;
			void*				defaultValue;

			~Description();
		};

	public: // Methods
							TCShaderUniform( TCGraphicsContext* context );
							TCShaderUniform( const TCShaderUniform& toCopy );
							~TCShaderUniform();
		TCShaderUniform&	operator=( const TCShaderUniform& toCopy );

		TCResult			Initialize( Description* description );
		TCResult			SetValue( void* data, unsigned int dataSize );

		TCResult			BindValue();
		TCResult			Release();

		const TCString&		GetName()					{ return mName; }
		const Type			GetType()					{ return mType; }
		const Usage			GetUsage()					{ return mUsage; }
		const void*			GetDefaultData()			{ return mDefaultValue; }
		const void*			GetData()					{ return mData; }

		unsigned int		GetDataSize()				{ return GetDataSize( GetType() ); }
		
		static unsigned int	GetDataSize( Type type );
		static TCString		TypeToString( Type type );
		static Type			StringToType( TCString& typeString );
		static TCString		UsageToString( Usage usage );
		static Usage		StringToUsage( TCString& usageString );

	protected: // Members
		TCString							mName;
		Type								mType;
		Usage								mUsage;
		void*								mData;
		void*								mDefaultValue;
		unsigned int						mRegister;
		TCConstantBuffer*					mParentConstantBuffer;
		TCGraphicsContext::TCShaderContext* mContext;

	protected: // Methods

		void				Clone( const TCShaderUniform& toCopy );

		// Friends!
		friend class TCGraphicsContext;
		friend class TCGraphicsContext_DX11;
		friend class TCConstantBuffer;
};

#endif //__TC_SHADER_UNIFORM_H__