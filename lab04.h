/*--------------------------------------------------------------------------------------------*\
CSI 4133 Computer methods for picture processing and analysis
Winter, 2015
\*--------------------------------------------------------------------------------------------*/
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#if !defined LAB4
#define LAB4

using namespace std;
using namespace cv;

class lab04
{
public:
    lab04();
	void readImage();
	void compression();
	void decompression();

private:
    void normalizeImg(Mat &img);
    void normalizeMat(Mat &img);
	Mat imgOriginal;
    Mat LuminanceQuantizationTable;
    Mat ChrominanceQuantizationTable;
};

#endif