#include <p2gz/images.h>

using namespace gz;

Image::Image(const ResTIMG* timg, const char* name_)
    : name(name_)
{
	image = new J2DPictureEx(timg, 0x20000);
}

Image::~Image()
{
	delete image;
	image = nullptr;
}

f32 Image::draw(f32 x, f32 z, f32 width, f32 height)
{
	if (!image) {
		return 0.0f;
	}

	image->draw(x, z, width, height, false, false, false);
	return width;
}

ImageMgr::ImageMgr()
    : menu_width(32.0f)
    , menu_height(32.0f)
    , menu_spacing(5.0f)
{
}

void ImageMgr::init()
{
	JKRArchive* image_arc = JKRMountArchive("menu_images.szs", JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
	GZASSERTLINE(image_arc);

	push(new Image(getImageFile(image_arc, "l_btn.bti"), "l_btn"));
	push(new Image(getImageFile(image_arc, "r_btn.bti"), "r_btn"));
	push(new Image(getImageFile(image_arc, "x_btn.bti"), "x_btn"));
	push(new Image(getImageFile(image_arc, "y_btn.bti"), "y_btn"));

	push(new Image(getImageFile(image_arc, "rp_l64.bti"), "red_leaf"));
	push(new Image(getImageFile(image_arc, "rp_b64.bti"), "red_bud"));
	push(new Image(getImageFile(image_arc, "rp_f64.bti"), "red_flower"));

	push(new Image(getImageFile(image_arc, "yp_l64.bti"), "yellow_leaf"));
	push(new Image(getImageFile(image_arc, "yp_b64.bti"), "yellow_bud"));
	push(new Image(getImageFile(image_arc, "yp_f64.bti"), "yellow_flower"));

	push(new Image(getImageFile(image_arc, "bp_l64.bti"), "blue_leaf"));
	push(new Image(getImageFile(image_arc, "bp_b64.bti"), "blue_bud"));
	push(new Image(getImageFile(image_arc, "bp_f64.bti"), "blue_flower"));

	push(new Image(getImageFile(image_arc, "wp_l64.bti"), "white_leaf"));
	push(new Image(getImageFile(image_arc, "wp_b64.bti"), "white_bud"));
	push(new Image(getImageFile(image_arc, "wp_f64.bti"), "white_flower"));

	push(new Image(getImageFile(image_arc, "blp_l64.bti"), "purple_leaf"));
	push(new Image(getImageFile(image_arc, "blp_b64.bti"), "purple_bud"));
	push(new Image(getImageFile(image_arc, "blp_f64.bti"), "purple_flower"));

	push(new Image(getImageFile(image_arc, "cha_l.bti"), "bulbmin_leaf"));
	push(new Image(getImageFile(image_arc, "cha_b.bti"), "bulbmin_bud"));
	push(new Image(getImageFile(image_arc, "cha_f.bti"), "bulbmin_flower"));

	push(new Image(getImageFile(image_arc, "onion_blue.bti"), "onion_blue"));
	push(new Image(getImageFile(image_arc, "onion_red.bti"), "onion_red"));
	push(new Image(getImageFile(image_arc, "onion_yellow.bti"), "onion_yellow"));
	push(new Image(getImageFile(image_arc, "ship_purple.bti"), "ship_purple"));
	push(new Image(getImageFile(image_arc, "ship_white.bti"), "ship_white"));
}

const ResTIMG* ImageMgr::getImageFile(JKRArchive* arc, const char* file_path)
{
	ResTIMG* img = (ResTIMG*)arc->getResource(file_path);
	GZASSERTLINE(img);

	return img;
}

f32 ImageMgr::draw(const char* name, f32 x, f32 z)
{
	Image* img = get(name);

	if (!img) {
		return 0.0f;
	}
	j3dSys.drawInit();
	GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
	sys->mGfx->mOrthoGraph.setPort();
	GXSetAlphaUpdate(GX_TRUE);
	GXSetColorUpdate(GX_TRUE);

	return img->draw(x, z, menu_width, menu_height);
}
