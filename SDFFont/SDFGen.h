#pragma once

struct ImageHandler
{
	void* pixels;	//Only RGB24 bitmap
	int stride;
	int w, h;
};

void GenSDF(ImageHandler src, ImageHandler sdf, int px01);