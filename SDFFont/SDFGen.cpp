#include "SDFGen.h"
#include <stdint.h>
#include <cmath>
#include <omp.h>

void GenSDF(ImageHandler src, ImageHandler sdf, int px01)
{
	const uint8_t* srcPx = static_cast<uint8_t*>(src.pixels);
	uint8_t* sdfPx = static_cast<uint8_t*>(sdf.pixels);

	#pragma omp parallel for
	for (int y = 0; y < sdf.h; ++y)
	{
		for (int x = 0; x < sdf.w; ++x)
		{
			int distance = px01*px01;

			for (int sy = -px01 + y; sy <= px01 + y; ++sy)
			{
				if (sy < 0 || sy >= src.h) continue;

				for (int sx = -px01 + x; sx <= px01 + x; ++sx)
				{
					if (sx < 0 || sx >= src.w) continue;

					const int srcOffset = src.stride*sy + sx * 4;
					const uint8_t* srcPxa = &srcPx[srcOffset];
					if (srcPxa[0] > 127)
					{
						const int xoff = sx - x;
						const int yoff = sy - y;
						const int thisDis = xoff*xoff + yoff*yoff;
						if (thisDis < distance)
							distance = thisDis;
					}
				}
			}

			double zoDistance = sqrt(double(distance)) / px01;

			if (zoDistance < 0) zoDistance = 0;
			if (zoDistance > 1) zoDistance = 1;
			
			const uint8_t* srcPxInThisPos = srcPx + y * src.stride + x * 4;

			
			const uint8_t orgPixelOutput = srcPxInThisPos[1];
			const uint8_t edge = srcPxInThisPos[0];

			if (orgPixelOutput > 127)
				zoDistance = -zoDistance;

			zoDistance = zoDistance / 2 + 0.5;


			const uint8_t distanceOutput = 255 - uint8_t(zoDistance * 255);

			sdfPx[src.stride*y + x * 4] = distanceOutput;	//B
			sdfPx[src.stride*y + x * 4 + 1] = distanceOutput;	//G
			sdfPx[src.stride*y + x * 4 + 2] = distanceOutput;	//unused	R
			sdfPx[src.stride*y + x * 4 + 3] = 255;	//unused
		}
	}
}
