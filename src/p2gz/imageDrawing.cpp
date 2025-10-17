#include <p2gz/ImageDrawing.h>
#include <p2gz/p2gz.h>
#include <JSystem/JKernel/JKRArchive.h>
#include <JSystem/J3D/J3DSys.h>
#include <System.h>
#include <Graphics.h>

using namespace gz;

Image::Image(const char* title_, const char* archive_path_, const char* file_path_)
    : title(title_)
    , archive_path(archive_path_)
    , file_path(file_path_)
    , image(nullptr)
{
}

void Image::load()
{
	if (!image) {
		JKRArchive* arc = JKRMountArchive(archive_path, JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
		GZASSERTLINE(arc);

		ResTIMG* img = (ResTIMG*)arc->getResource(file_path);
		GZASSERTLINE(img);

		image = new J2DPictureEx(img, 0x20000);
		image->resize(p2gz->imageMgr->width(), p2gz->imageMgr->height());
	}
}

void Image::deload()
{
	if (image) {
		delete image;
		image = nullptr;
	}
}

f32 Image::draw(f32 x, f32 z)
{
	if (image) {
		// prepare GX unit for drawing image
		j3dSys.drawInit();
		GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
		sys->mGfx->mOrthoGraph.setPort();
		GXSetAlphaUpdate(GX_TRUE);
		GXSetColorUpdate(GX_TRUE);

		// draw image
		image->draw(x, z, width, height, false, false, false);
		return width;
	}
	return 0.0f;
}

void ImageMgr::init()
{
	// Pikmin type icons
	push(new Image("red_leaf", "new_screen/eng/res_cave.szs", "timg/rp_l64.bti"));
	push(new Image("red_bud", "new_screen/eng/res_cave.szs", "timg/rp_b64.bti"));
	push(new Image("red_flower", "new_screen/eng/res_cave.szs", "timg/rp_f64.bti"));
}

Image* ImageMgr::get(const char* title_)
{
	for (size_t i = 0; i < menu_images.len(); i++) {
		Image* img = menu_images[i];
		if (img && (strcmp(menu_images[i]->title, title_) == 0)) {
			return img;
		}
	}
	return nullptr;
}

void ImageMgr::remove(const char* title_)
{
	for (size_t i = 0; i < menu_images.len(); i++) {
		Image* img = menu_images[i];
		if (img && (strcmp(menu_images[i]->title, title_) == 0)) {
			// removeAt doesn't delete the object, so manually delete it first
			delete img;
			menu_images.removeAt(i);
			return;
		}
	}
}

void ImageMgr::load(const char* title_)
{
	for (size_t i = 0; i < menu_images.len(); i++) {
		Image* img = menu_images[i];
		if (img && (strcmp(img->title, title_) == 0)) {
			img->load();
			return;
		}
	}
}

void ImageMgr::load_all()
{
	for (size_t i = 0; i < menu_images.len(); i++) {
		Image* img = menu_images[i];
		if (img) {
			img->load();
		}
	}
}

void ImageMgr::deload(const char* title_)
{
	for (size_t i = 0; i < menu_images.len(); i++) {
		Image* img = menu_images[i];
		if (img) {
			img->deload();
			return;
		}
	}
}

void ImageMgr::deload_all()
{
	for (size_t i = 0; i < menu_images.len(); i++) {
		Image* img = menu_images[i];
		if (img) {
			img->deload();
		}
	}
}

void ImageMgr::bump_menu_image_size(f32 widthAmt, f32 heightAmt)
{
	menu_image_width += widthAmt;
	if (menu_image_width < 0.0f) {
		menu_image_width = 0.0f;
	}
	menu_image_height += heightAmt;
	if (menu_image_height < 0.0f) {
		menu_image_height = 0.0f;
	}

	resize_all();
}

void ImageMgr::set_menu_image_size(f32 width, f32 height)
{
	menu_image_width  = width;
	menu_image_height = height;

	resize_all();
}

void ImageMgr::resize_all()
{
	for (size_t i = 0; i < menu_images.len(); i++) {
		Image* img = menu_images[i];
		if (img) {
			img->resize(menu_image_width, menu_image_height);
		}
	}
}
