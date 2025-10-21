#ifndef _IMAGES_H
#define _IMAGES_H

#include <types.h>
#include <p2gz/gzMacros.h>
#include <p2gz/gzCollections.h>
#include <JSystem/J3D/J3DSys.h>
#include <JSystem/J2D/J2DPicture.h>
#include <JSystem/JKernel/JKRArchive.h>
#include <System.h>
#include <Graphics.h>

namespace gz {

struct Image {
	Image(const ResTIMG* timg, const char* name_);

	~Image();

	void draw(f32 x, f32 y, f32 width, f32 height);

	bool is(const char* other_name) { return strcmp(name, other_name) == 0; }

private:
	J2DPictureEx* image;
	const char* name;
};

struct ImageMgr {
	ImageMgr() { }
	~ImageMgr() { }

	/// To add a new image, add it to this function with an appropriate name.
	void init();

	Image* get(const char* name)
	{
		for (size_t i = 0; i < images.len(); i++) {
			Image* img = images[i];
			if (img->is(name)) {
				return img;
			}
		}
		return nullptr;
	}

	void push(Image* new_image) { images.push(new_image); }

	void draw(const char* name, f32 x, f32 y, f32 width, f32 height)
	{
		Image* img = get(name);

		j3dSys.drawInit();
		GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
		sys->mGfx->mOrthoGraph.setPort();
		GXSetAlphaUpdate(GX_TRUE);
		GXSetColorUpdate(GX_TRUE);

		img->draw(x, y, width, height);
	}

	/// Get ResTIMG image out of menu_images archive, once loaded
	static const ResTIMG* getImageFile(JKRArchive* arc, const char* file_path);

private:
	Vec<Image*> images;
};

} // namespace gz

#endif
