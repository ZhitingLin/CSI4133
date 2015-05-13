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

lab04::lab04(){
    float Ldata[] =
    {16,  11,  10,  16,  24,  40,  51,  61,
        12,  12,  14,  19,  26,  58,  60,  55,
        14,  13,  16,  24,  40,  57,  69,  56,
        14,  17,  22,  29,  51,  87,  80,  62,
        18,  22,  37,  56,  68, 109, 103,  77,
        24,  35,  55,  64,  81, 104, 113,  92,
        49,  64,  78,  87, 103, 121, 120, 101,
        72,  92,  95,  98, 112, 100, 103,  99};
    LuminanceQuantizationTable = Mat(8, 8, CV_32FC1, Ldata).clone();
    float Cdata[] =
    {17,  18,  24,  47,  99,  99,  99,  99,
        18,  21,  26,  66,  99,  99,  99,  99,
        24,  26,  56,  99,  99,  99,  99,  99,
        47,  66,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99};
    ChrominanceQuantizationTable = Mat(8, 8, CV_32FC1, Cdata).clone();
}

void lab04::readImage()
{
    imgOriginal = imread("YOUR IMAGE address");
    imshow("Original Image", imgOriginal);
    waitKey(0);
}

void lab04::compression()
{
    Mat Compressed;
    imgOriginal.copyTo(Compressed);
    // convert the color space
    cvtColor(Compressed, Compressed, CV_BGR2YCrCb);
    
    normalizeImg(Compressed);
    
    Mat standarTable[3] = {LuminanceQuantizationTable,
        ChrominanceQuantizationTable,
        ChrominanceQuantizationTable};
    
    int width = Compressed.cols;
    int height = Compressed.rows;
    // split the image into block
    for (int i = 0; i<width; i+=8 ) {
        for (int j = 0; j < height; j+=8 ) {
            // create the 8*8 block
            cv::Rect roi(i, j, 8, 8);
            Compressed.convertTo(Compressed, CV_32FC1);
            cv::Mat block = Compressed(roi);
            vector<Mat> YCrCblayers;
            split(block, YCrCblayers);
            for (int k = 0; k<YCrCblayers.size(); k++) {
                //DCT
                //convert the layers into float.
                subtract(YCrCblayers[k],128, YCrCblayers[k]);
                // do the discrete cosin transform
                dct(YCrCblayers[k], YCrCblayers[k]);
                //quantization:
                divide(YCrCblayers[k], standarTable[k], YCrCblayers[k]);
                normalizeMat(YCrCblayers[k]);
            }
            merge(YCrCblayers, block);
        }
    }
    //store data
    cv::FileStorage storage("temp", cv::FileStorage::WRITE);
    storage << "img" << Compressed;
    storage.release();
}

void lab04::normalizeImg(Mat& img){
    int width = img.cols + img.cols%8;
    int height = img.rows + img.rows%8;

    copyMakeBorder(img, img, 0, height-imgOriginal.rows, 0, width-imgOriginal.cols, IPL_BORDER_REPLICATE);
}

void lab04::normalizeMat(cv::Mat& img){
    for(int row = 0; row < img.rows; ++row) {
        for(int col = 0; col < img.cols; ++col) {
            img.at<float>(row, col) = round(img.at<float>(row, col));
        }
    }
}

void lab04::decompression()
{
    //read back data.
    Mat decompressedImg;
    cv::FileStorage storage("temp", cv::FileStorage::READ);
    storage["img"] >> decompressedImg;
    storage.release();
    
    Mat standarTable[3] = {LuminanceQuantizationTable,
        ChrominanceQuantizationTable,
        ChrominanceQuantizationTable};
    
    int width = decompressedImg.cols;
    int height = decompressedImg.rows;
    decompressedImg.convertTo(decompressedImg, CV_32FC1);
    // split the image into block
    if (width%2 == 0 && height %2 == 0) {
        for (int i = 0; i<width; i+=8 ) {
            for (int j = 0; j < height; j+=8 ) {
                // create the 8*8 block
                Mat block = decompressedImg(Rect(i, j, 8, 8));
                vector<Mat> YCrCblayers;
                split(block, YCrCblayers);
                for (int k = 0; k<YCrCblayers.size(); k++) {
                    //dequantization:
                    multiply(YCrCblayers[k], standarTable[k], YCrCblayers[k]);
                    //inverst discrete cosin transform.
                    idct(YCrCblayers[k], YCrCblayers[k]);
                    add(YCrCblayers[k],128, YCrCblayers[k]);
                }
                merge(YCrCblayers, block);
            }
        }
        decompressedImg.convertTo(decompressedImg, CV_8UC1);
        cvtColor(decompressedImg, decompressedImg, CV_YCrCb2BGR);
        imshow("Decompressed result image", decompressedImg);
        waitKey();
        
        imwrite("result.jpg", decompressedImg);
    }else{
        cout << "the compress img is not even number of height and width" << endl;
    }
    
    
}



