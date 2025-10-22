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

	f32 draw(f32 x, f32 z, f32 width, f32 height);

	bool is(const char* other_name) { return strcmp(name, other_name) == 0; }

private:
	J2DPictureEx* image;
	const char* name;
};

struct ImageMgr {
	ImageMgr();

	~ImageMgr() { delete &images; }

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

	f32 height() { return menu_height; }
	f32 spacing() { return menu_spacing; }

	void push(Image* new_image) { images.push(new_image); }

	f32 draw(const char* name, f32 x, f32 z);

	/// Get ResTIMG image out of menu_images archive, once loaded
	static const ResTIMG* getImageFile(JKRArchive* arc, const char* file_path);

private:
	Vec<Image*> images;
	// consistent widths and heights for images in the menu
	f32 menu_width;
	f32 menu_height;
	f32 menu_spacing;
};

} // namespace gz

#endif
