// Copyright (C) 2009-2010 Amundis
// Heavily based on the OpenGL driver implemented by Nikolaus Gebhardt
// and OpenGL ES driver implemented by Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h
#include "COGLES2Renderer2D.h"
#include <jni.h>
#include <string.h>

extern irr::core::stringc gSdCardPath;

#ifdef _IRR_COMPILE_WITH_OGLES2_

namespace irr
{
    namespace video
    {

        const char* const COGLES2Renderer2d::sBuiltInShaderUniformNames[] =
        {
            "uOrthoMatrix",
            "uUseTexture",
            "uTextureUnit",
            "uAlphaTest",
            "uAlphaValue",
            0
        };


        COGLES2Renderer2d::COGLES2Renderer2d( irr::video::COGLES2Driver *driver, irr::io::IFileSystem *fs )
                : COGLES2SLMaterialRenderer( driver, fs, 0, 0, sBuiltInShaderUniformNames, UNIFORM_COUNT )
        {
#ifdef _DEBUG
            setDebugName( "COGLES2Renderer2d" );
#endif
            s32 dummy = -1;

			strcpy(VertexShaderFile, gSdCardPath.c_str());
			strcat(VertexShaderFile,"/Irrlicht/COGLES2Renderer2D.vsh");
			strcpy( FragmentShaderFile, gSdCardPath.c_str());
			strcat( FragmentShaderFile,"/Irrlicht/COGLES2Renderer2D.fsh");

            initFromFiles( dummy, VertexShaderFile, FragmentShaderFile, false );
            useProgram();
            int texUnit = 0;
            setUniform( TEXTURE_UNIT, &texUnit );
        }

        void COGLES2Renderer2d::useTexture( bool param )
        {
            if ( param != UseTexture )
            {
                UseTexture = param;
                int dummy = param ? 1 : 0;
                setUniform( USE_TEXTURE, &dummy );
            }
        }

        void COGLES2Renderer2d::useAlphaTest( bool param )
        {
            if ( param != UseAlphaTest )
            {
                UseAlphaTest = param;
                int dummy = param ? 1 : 0;
                setUniform( ALPHA_TEST, &dummy );
            }
        }

        void COGLES2Renderer2d::setAlphaTestValue( float value )
        {
            if ( value != AlphaTestValue )
            {
                AlphaTestValue = value;
                setUniform( ALPHA_VALUE, &AlphaTestValue );
            }
        }

        void COGLES2Renderer2d::setOrthoMatrix( const core::matrix4 &matrix )
        {
            if ( matrix != OrthoMatrix )
            {
                OrthoMatrix = matrix;
                setUniform( ORTHO_MATRIX, OrthoMatrix.pointer() );
            }
        }
    }
}
#endif
