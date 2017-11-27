
#include "SDFGenerator.h"
#include "SDFGen.h"

using namespace System::Drawing;




SDFGenerator::SDFGenerator(int pxDistanceTo01Range)
{
	pxDistanceTo01Range_ = pxDistanceTo01Range;
}

System::Drawing::Bitmap ^ SDFGenerator::GenSDF(System::Drawing::Bitmap ^ org)
{


	/*Bitmap ^ sdf = gcnew Bitmap(
		int(2 * pxDistanceTo01Range_ + orgClipped->Width),
		int(2 * pxDistanceTo01Range_ + orgClipped->Height)
	);*/

	Bitmap ^ sdf = gcnew Bitmap(
		int(org->Width),
		int(org->Height)
	);

	System::Drawing::Rectangle sdfRect;
	sdfRect.X = 0;
	sdfRect.Y = 0;
	sdfRect.Width = sdf->Width;
	sdfRect.Height = sdf->Height;
	auto sdfData = sdf->LockBits(
		sdfRect, 
		Imaging::ImageLockMode::ReadWrite, 
		Imaging::PixelFormat::Format32bppRgb
	);

	//auto org = gcnew Bitmap(sdf->Width, sdf->Height);
	/*auto grap = Graphics::FromImage(org);
	grap->DrawImage(orgClipped, pxDistanceTo01Range_, pxDistanceTo01Range_);
	grap->Flush();*/

	System::Drawing::Rectangle orgRect;
	orgRect.X = 0;
	orgRect.Y = 0;
	orgRect.Width = org->Width;
	orgRect.Height = org->Height;
	auto orgData = org->LockBits(
		orgRect,
		Imaging::ImageLockMode::ReadOnly,
		Imaging::PixelFormat::Format32bppRgb
	);

	ImageHandler sdfH;
	sdfH.w = sdfRect.Width;
	sdfH.h = sdfRect.Height;
	sdfH.stride = sdfData->Stride;
	sdfH.pixels = sdfData->Scan0.ToPointer();

	ImageHandler orgH;
	orgH.w = orgRect.Width;
	orgH.h = orgRect.Height;
	orgH.stride = orgData->Stride;
	orgH.pixels = orgData->Scan0.ToPointer();

	::GenSDF(orgH, sdfH,pxDistanceTo01Range_);

	sdf->UnlockBits(sdfData);
	org->UnlockBits(orgData);
	//grap->~Graphics();
	org->~Bitmap();
	//orgClipped->~Bitmap();
	return sdf;
}
