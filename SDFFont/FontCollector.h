#pragma once
#include "FontFace.h"
ref class FontCollector
{
private:
	int width_, height_,fontHeight_;
	System::String^ output_;
	int faceID_;
	FontFace^ face_;
	int stdHeight_;

	System::Text::StringBuilder sb_;
public:
	FontCollector(int width, int height,int fontHeight,System::String^ output,int stdHeight);
	~FontCollector();
	void AddChar(System::Drawing::Bitmap^ bitmap,wchar_t ch);
};
