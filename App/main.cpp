#include <CImg.h>
#include <algorithm>
#include <iostream>

using namespace cimg_library;

#define max(a, b) ((a)>(b) ? (a) : (b))

int main() {
	CImg<float> original, image("lena.png");
	original = image;
	CImgDisplay main_display(image, "Lena");

	unsigned int	minHue = 360,
					maxHue = 0;

	float	minSat = 1,
			maxSat = 0,
			minVal = 1,
			maxVal = 0;

	unsigned int dHue = 0;
	float dSat = 1;
	float dVal = 1;

	original.RGBtoHSV();

	cimg_forXY(original, x, y) {
		if (original(x, y, 0) < minHue)
			minHue = original(x, y, 0);
		if (original(x, y, 0) > maxHue)
			maxHue = original(x, y, 0);

		if (original(x, y, 1) < minSat)
			minSat = original(x, y, 1);
		if (original(x, y, 1) > maxSat)
			maxSat = original(x, y, 1);

		if (original(x, y, 2) < minVal)
			minVal = original(x, y, 2);
		if (original(x, y, 2) > maxVal)
			maxVal = original(x, y, 2);
	}

	std::cout << "Hue:\n" << "\tMin: " << minHue << "\tMax: " << maxHue << std::endl;
	std::cout << "Sat:\n" << "\tMin: " << minSat << "\tMax: " << maxSat << std::endl;
	std::cout << "Val:\n" << "\tMin: " << minVal << "\tMax: " << maxVal << std::endl;

	while(!main_display.is_closed()) {
		main_display.wait();

		image = original;

		if (main_display.is_keyQ())
			dHue += 5;
		else if (main_display.is_keyA())
			dHue -= 5;

		else if (main_display.is_keyW())
			dSat *= 1.1;
		else if (main_display.is_keyS())
			dSat /= 1.1;

		else if (main_display.is_keyE())
			dVal *= 1.1;
		else if (main_display.is_keyD())
			dVal /= 1.1;

		std::cout << "dHue: " << dHue << std::endl;
		std::cout << "dSat: " << dSat << std::endl;
		std::cout << "dVal: " << dVal << std::endl;

		// loop to adjust H, S, or V here
		cimg_forXY(image, x, y) {
			// Shift Hue
			image(x, y, 0) = std::fmod(original(x, y, 0) + dHue, 360);

			// Shift Saturation
			image(x, y, 1) = original(x, y, 1)*dSat;

			// Shift Value
			image(x, y, 2) = original(x, y, 2)*dVal;
		}
		image.HSVtoRGB();
		main_display.display(image);
	}
	//image.HSVtoRGB().save("lena2.png");
}
