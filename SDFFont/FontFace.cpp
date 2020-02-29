#include "FontFace.h"
using namespace System::Drawing;
using namespace System;


FontFace::FontFace(int width, int height)
{
	image_ = gcnew Bitmap(width, height);
	graphic_ = Graphics::FromImage(image_);

	lineX_ = 0;
	lineY_ = 0;

	lineHeight_ = 0;
}

bool FontFace::AddChar(System::Drawing::Bitmap ^ bitmap, int fontHeight, System::Drawing::Rectangle & rectOutput)
{
	const double aspect = double(bitmap->Width) / double(bitmap->Height);
	rectOutput.X = lineX_;
	rectOutput.Y = lineY_;
	rectOutput.Height = fontHeight;
	rectOutput.Width = int(aspect * rectOutput.Height);

	if (rectOutput.X + rectOutput.Width >= image_->Width)
	{
		lineX_ = 0;
		lineY_ += lineHeight_;
		lineHeight_ = 0;

		rectOutput.X = 0;
	}

	if (fontHeight > lineHeight_) lineHeight_ = fontHeight;

	if (lineY_ + rectOutput.Height >= image_->Height)
		return false;

	lineX_ += rectOutput.Width;

	rectOutput.Y = lineY_;
	graphic_->DrawImage(bitmap, rectOutput);
	bitmap->~Bitmap();
	return true;
}

void FontFace::SaveTo(System::String ^ output)
{
	System::Console::WriteLine("Writing... " + output);
	graphic_->Flush();

	int maxX = 0, maxY = 0;
	for (int y = 0; y < image_->Height; ++y)
	{
		for (int x = 0; x < image_->Width; ++x)
		{
			if (image_->GetPixel(x, y).A >= 1)
			{
				if (x > maxX) maxX = x;
				if (y > maxY) maxY = y;
			}
		}
	}

	maxX++; maxY++;

	auto pCliped = gcnew Bitmap(maxX,maxY,image_->PixelFormat);
	for (int y = 0; y < maxY; ++y)
		for (int x = 0; x < maxX; ++x)
			pCliped->SetPixel(x, y, image_->GetPixel(x, y));
	

	pCliped->Save(output);
}

