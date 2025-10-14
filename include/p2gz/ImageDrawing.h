#ifndef _GZ_IMAGEDRAWING_H
#define _GZ_IMAGEDRAWING_H

#include <types.h>
#include <JSystem/JUtility/TColor.h>
#include <JSystem/J2D/J2DPicture.h>
#include <p2gz/gzCollections.h>

namespace gz {

struct Image {
public:
	Image(const char* archive_path_, const char* file_path_);

	~Image() { delete image; }

	void load();
	void draw();

	f32 get_x() { return x; }
	f32 get_y() { return y; }
	f32 get_width() { return width; }
	f32 get_height() { return height; }

	void set(f32 x_, f32 y_, f32 width_, f32 height_)
	{
		x      = x_;
		y      = y_;
		width  = width_;
		height = height_;
	}

	void move(f32 x_, f32 y_)
	{
		x = x_;
		y = y_;
	}

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

private:
	const char* archive_path; // path to archive.szs (within files/)
	const char* file_path;    // path to file.bti (within archive.szs)

	f32 x;
	f32 y;
	f32 width;
	f32 height;

	J2DPictureEx* image;
};

}; // namespace gz

#endif
