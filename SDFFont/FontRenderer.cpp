
#include "FontRenderer.h"
#include <stdint.h>
#include <omp.h>
using namespace System;
using namespace System::Drawing;

FontRenderer::FontRenderer(System::String ^ fontName):
	font_(fontName,384,System::Drawing::GraphicsUnit::Pixel)
{
	
}

Bitmap ^ FontRenderer::Render(wchar_t ch,bool nosdf)
{
	Bitmap^ bitmap = gcnew Bitmap(512,512);
	Graphics^ grap = Graphics::FromImage(bitmap);
	grap->Clear(System::Drawing::Color::Black);

	wchar_t src[] = { ch,0 };
	String^ s = gcnew String(src);
	s = s->Trim();

	Drawing::Rectangle r;
	r.X = 0;
	r.Y = 0;
	r.Width = bitmap->Width;
	r.Height = bitmap->Height;

	auto size = grap->MeasureString(s, %font_);
	grap->DrawString(s, %font_, Brushes::White,r);

	if (!nosdf)
	{

		auto lockedData = bitmap->LockBits(r, Imaging::ImageLockMode::ReadWrite, Imaging::PixelFormat::Format24bppRgb);
		uint8_t* pixels = static_cast<uint8_t*>(lockedData->Scan0.ToPointer());

		int top = -1, bottom = 0;

		for (int y = 0; y < lockedData->Height; ++y)
		{
			for (int x = 0; x < lockedData->Width; ++x)
			{
				uint8_t* thisPx = pixels + y*lockedData->Stride + x * 3;

				//B 0 - 是否为边缘
				//G 1 - 是否在内部

				//R 2 - 保留做将来使用
				thisPx[2] = 0;

				bool isZero = thisPx[1] < 127;
				bool isEdge = false;
				bool keepFind = true;
				if (!isZero)
				{
					if (top == -1)
						top = y;
					bottom = y;
					for (int xO = -1; xO <= 1 && keepFind; ++xO)
					{
						for (int yO = -1; yO <= 1; ++yO)
						{
							int nx = x + xO;
							int ny = y + yO;
							if (nx < 0 || nx >= lockedData->Width || ny < 0 || ny >= lockedData->Height)
								break;
							uint8_t* thisPx = pixels + ny*lockedData->Stride + nx * 3;
							if (thisPx[1] < 127)
							{
								keepFind = false;
								isEdge = true;
							}
						}
					}
				}

				thisPx[0] = isEdge ? 255 : 0;
			}
		}

		bitmap->UnlockBits(lockedData);
		grap->~Graphics();
	}
	
	Bitmap^ cliped = gcnew Bitmap(int(size.Width),int(size.Height));
	grap = Graphics::FromImage(cliped);


	r.X = 0;
	r.Y = 0;
	r.Width = int(size.Width);
	r.Height = int(size.Height);

	grap->DrawImageUnscaledAndClipped(bitmap,r);
	grap->Flush();
	grap->~Graphics();

	bitmap->~Bitmap();
	return cliped;
}
