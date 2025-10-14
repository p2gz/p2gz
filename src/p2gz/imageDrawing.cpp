#include <p2gz/ImageDrawing.h>
#include <JSystem/JKernel/JKRArchive.h>
#include <JSystem/J3D/J3DSys.h>

using namespace gz;

Image::Image(const char* archive_path_, const char* file_path_)
    : archive_path(archive_path_)
    , file_path(file_path_)
    , image(nullptr)
{
	load();
}

void Image::load()
{
	if (!image) {
		JKRArchive* arc = JKRMountArchive(archive_path, JKRArchive::EMM_Mem, nullptr, JKRArchive::EMD_Head);
		GZASSERTLINE(arc);

		ResTIMG* img = (ResTIMG*)arc->getResource(file_path);
		GZASSERTLINE(img);

		image = new J2DPictureEx(img, 0x20000);
	}
}

void Image::draw()
{
	if (image) {
		image->draw(x, y, width, height, false, false, false);
	}
}
