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
    : menu_width(20.0f)
    , menu_height(20.0f)
    , menu_spacing(5.0f)
{
}

void ImageMgr::init()
{
	JKRArchive* image_arc = JKRMountArchive("menu_images.szs", JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
	GZASSERTLINE(image_arc);

	push(new Image(getImageFile(image_arc, "rp_l64.bti"), "red_leaf"));
	push(new Image(getImageFile(image_arc, "rp_b64.bti"), "red_bud"));
	push(new Image(getImageFile(image_arc, "rp_f64.bti"), "red_flower"));
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
