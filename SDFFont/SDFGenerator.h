#pragma once

class SDFGenerator
{
private:
	int pxDistanceTo01Range_;

public:
	SDFGenerator(int pxDistanceTo01Range);

	System::Drawing::Bitmap^ GenSDF(System::Drawing::Bitmap^ org);
};