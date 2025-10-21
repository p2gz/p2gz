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

void Image::draw(f32 x, f32 y, f32 width, f32 height)
{
	if (image) {
		image->draw(x, y, width, height, false, false, false);
	}
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
