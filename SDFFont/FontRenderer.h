#pragma once
ref class FontRenderer
{
private:
	System::Drawing::Font font_;
public:
	FontRenderer(System::String^ fontName);
	System::Drawing::Bitmap^ Render(wchar_t ch,bool nosdf);
};