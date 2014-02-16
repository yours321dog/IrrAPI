// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_OGLES1_EXTERNAL_TEXTURE_H_INCLUDED__
#define __C_OGLES1_EXTERNAL_TEXTURE_H_INCLUDED__

#include "ITexture.h"
#include "IImage.h"

#include "IrrCompileConfig.h"
#if defined(_IRR_COMPILE_WITH_OGLES1_)

#if defined(_IRR_COMPILE_WITH_IPHONE_DEVICE_)
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#elif defined(_IRR_ANDROID_PLATEFORM_)
#include "importgl.h"
#else
#include <GLES/egl.h>
#endif

namespace irr
{
namespace video
{

class COGLES1Driver;
//! OGLES1 external texture.
class COGLES1ExternalTexture : public COGLES1Texture
{
public:

	//! constructor
	COGLES1ExternalTexture(IImage* surface, const io::path& name, COGLES1Driver* driver=0, void* mipmapData=0);

	//! destructor
	virtual ~COGLES1ExternalTexture();

	//! lock function
	virtual void* lock(bool readOnly = false, u32 mipmapLevel=0);

	//! unlock function
	virtual void unlock();

	//! Returns original size of the texture (image).
	virtual const core::dimension2d<u32>& getOriginalSize() const;

	//! Returns size of the texture.
	virtual const core::dimension2d<u32>& getSize() const;

	//! returns color format of texture
	virtual ECOLOR_FORMAT getColorFormat() const;

	//! returns pitch of texture (in bytes)
	virtual u32 getPitch() const;

	//! return whether this texture has mipmaps
	virtual bool hasMipMaps() const;

	//! Regenerates the mip map levels of the texture.
	virtual void regenerateMipMapLevels(void* mipmapData=0);

	//! Is it a render target?
	virtual bool isRenderTarget() const;

	//! Is it a FrameBufferObject?
	virtual bool isFrameBufferObject() const;

protected:

	//! protected constructor with basic setup, no GL texture name created, for derived classes
	COGLES1ExternalTexture(const io::path& name, COGLES1Driver* driver);

	//! convert the image into an internal image with better properties for this driver.
	void getImageValues(IImage* image);

	//! copies the the texture into an open gl texture.
	void uploadTexture(bool newTexture=true, void* mipmapData=0, u32 mipLevel=0);
	
	
};

} // end namespace video
} // end namespace irr

#endif
#endif // __C_OGLES1_EXTERNAL_TEXTURE_H_INCLUDED__
