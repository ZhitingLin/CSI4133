/*--------------------------------------------------------------------------------------------*\
CSI 4133 Computer methods for picture processing and analysis
Winter, 2015
\*--------------------------------------------------------------------------------------------*/
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "lab04.h"

using namespace std;
using namespace cv;

int main()
{
	// Create assignment1 object
	lab04 zlin035; // your user name

	// Reading an image
	zlin035.readImage();
    zlin035.compression();

	// The rest of processing
    zlin035.decompression();
}

