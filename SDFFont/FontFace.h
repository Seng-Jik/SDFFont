#pragma once

ref class FontFace
{
private:
	System::Drawing::Bitmap^ image_;
	System::Drawing::Graphics^ graphic_;
	int lineX_, lineY_, lineHeight_;
public:
	FontFace(int width, int height);
	bool AddChar(System::Drawing::Bitmap^ bitmap, int fontHeight, System::Drawing::Rectangle& rectOutput);
	void SaveTo(System::String^ output);
};
