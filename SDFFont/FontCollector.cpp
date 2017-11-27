#include "FontCollector.h"
#include "SDFGenerator.h"
#include "FontRenderer.h"

FontCollector::FontCollector(int width, int height, int fontHeight, System::String ^ output,int stdHeight)
{
	width_ = width;
	height_ = height;
	fontHeight_ = fontHeight;
	output_ = output;
	faceID_ = 0;

	face_ = gcnew FontFace(width, height);
	stdHeight_ = stdHeight;
}

FontCollector::~FontCollector()
{
	face_->SaveTo(output_ + faceID_ + ".bmp");
	System::IO::File::WriteAllText(output_ + ".txt", sb_.ToString());
}

void FontCollector::AddChar(System::Drawing::Bitmap ^ bitmap,wchar_t ch)
{
	//double bl = float(stdHeight_) / bitmap->Height;

	System::Drawing::Rectangle rect;
	bool success = face_->AddChar(bitmap, fontHeight_, rect);

	if (!success)
	{
		face_->SaveTo(output_+faceID_+".bmp");
		face_ = gcnew FontFace(width_, height_);
		face_->AddChar(bitmap, fontHeight_, rect);
		faceID_++;
	}

	sb_.Append(ch);
	sb_.Append(L'\t');
	sb_.Append(L"faceID=");
	sb_.Append(faceID_);
	sb_.Append(L"\tx=");
	sb_.Append(rect.X);
	sb_.Append(L"\ty=");
	sb_.Append(rect.Y);
	sb_.Append(L"\tw=");
	sb_.Append(rect.Width);
	sb_.Append(L"\th=");
	sb_.Append(rect.Height);
	sb_.AppendLine();
}
