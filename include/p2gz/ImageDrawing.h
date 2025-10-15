#ifndef _GZ_IMAGEDRAWING_H
#define _GZ_IMAGEDRAWING_H

#include <types.h>
#include <JSystem/JUtility/TColor.h>
#include <JSystem/J2D/J2DPicture.h>
#include <p2gz/gzCollections.h>

namespace gz {

struct Image {
public:
	Image(const char* title_, const char* archive_path_, const char* file_path_);

	~Image() { delete image; }

	void load();
	void deload();

	// returns drawn image width
	f32 draw(f32 x, f32 z);

	f32 get_width() { return width; }
	f32 get_height() { return height; }

	void resize(f32 width_, f32 height_)
	{
		width  = width_;
		height = height_;
	}

	void show()
	{
		if (image) {
			image->show();
		}
	}

	void hide()
	{
		if (image) {
			image->hide();
		}
	}

	const char* title; // way to refer to image

private:
	const char* archive_path; // path to archive.szs (within files/)
	const char* file_path;    // path to file.bti (within archive.szs)

	f32 width;
	f32 height;

	J2DPictureEx* image;
};

struct ImageMgr {
public:
	ImageMgr()
	    : menu_images()
	    , menu_image_width(32.0f)
	    , menu_image_height(32.0f)
	    , menu_image_text_spacing(5.0f)
	{
	}

	void init();

	void push(Image* img) { menu_images.push(img); }

	f32 width() { return menu_image_width; }
	f32 height() { return menu_image_height; }
	f32 spacing() { return menu_image_text_spacing; }

	Image* get(const char* title_);
	void remove(const char* title_);

	void load(const char* title_);
	void load_all();

	void deload(const char* title_);
	void deload_all();

	void bump_menu_image_size(f32 widthAmt, f32 heightAmt);
	void set_menu_image_size(f32 width, f32 height);
	void resize_all();

private:
	Vec<Image*> menu_images;

	// to keep all menu images a consistent size
	f32 menu_image_width;
	f32 menu_image_height;
	f32 menu_image_text_spacing;
};

}; // namespace gz

#endif
