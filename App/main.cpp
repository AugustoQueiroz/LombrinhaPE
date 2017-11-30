#include <CImg.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "../../rtmidi/RtMidi.h"

//using namespace cimg_library;

#define max(a, b) ((a)>(b) ? (a) : (b))

unsigned int dHue = 0;
float dSat = 1;
float dVal = 1;

float dBlur = 0;


void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
	unsigned int nBytes = message->size();
	
	if (nBytes < 3) return;

	switch((int)message->at(1)) {
		case 3:
			dHue = (int)message->at(2)*360/128;
			break;
		
		case 4:
			dSat = (float)message->at(2)/128.0;
			break;
		
		case 5:
			dVal = 1 + (float)message->at(2)/128.0;
			break;
		
		case 6:
			dBlur = (float)message->at(2)*5/128.0;
			break;
	}

  /*unsigned int nBytes = message->size();
  for ( unsigned int i=0; i<nBytes; i++ )
	dHue = (int)message->at(i)*360/128;*/
	
//    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  //if ( nBytes > 0 )
    //std::cout << "stamp = " << deltatime << std::endl;
}

int main() {
	cimg_library::CImg<float> original, image("lena.png");
	original = image;
	cimg_library::CImgDisplay main_display(image, "Lena");

	RtMidiIn *midiin = new RtMidiIn();
	// Check available ports.
	unsigned int nPorts = midiin->getPortCount();
	if ( nPorts == 0 ) {
		std::cout << "No ports available!\n";
		goto cleanup;
	}
	midiin->openPort( 1 );
	// Set our callback function.  This should be done immediately after
	// opening the port to avoid having incoming messages written to the
	// queue.
	midiin->setCallback( &mycallback );
	// Don't ignore sysex, timing, or active sensing messages.
	midiin->ignoreTypes( false, false, false );




	
	original.RGBtoHSV();


	while(!main_display.is_closed()) {
		main_display.wait(20);

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

		else if (main_display.is_keyR())
			dBlur += 0.1;
		else if (main_display.is_keyF())
			dBlur -= 0.1;

		if (dBlur < 0)
			dBlur = 0;

		std::cout << "dHue: " << dHue << std::endl;
		std::cout << "dSat: " << dSat << std::endl;
		std::cout << "dVal: " << dVal << std::endl;
		std::cout << "dBlur: " << dBlur << std::endl;

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

		image.blur(dBlur);

		main_display.display(image);
	}
	//image.HSVtoRGB().save("lena2.png");
	cleanup:
	delete midiin;
	return 0;

}
