#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 * 2. 编写一个基于直方图技术的阈值分割函数threshTwoPeaks(),并用实验图像1和实验图像2来验证分割函数。
 * 参考书P190
 *
 */

using namespace std;
using namespace cv;

// 计算图像的灰度直方图 参考书P99
Mat calcGrayHist(const Mat &image){
    // 存储256个灰度级的像素个数
    Mat histogram = Mat::zeros(Size(256,1), CV_32SC1);
    // 图像的高和宽
    int rows = image.rows;
    int cols = image.cols;
    // 计算每个灰度级的个数
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            int index = int(image.at<uchar>(r,c));
            histogram.at<int>(0, index) += 1;
        }
    }
    return histogram;
}

// 参考书P190
int threshTwoPeaks(const Mat & image, Mat & thresh_out){
    // 计算灰度直方图
    Mat histogram = calcGrayHist(image);
    // 找到灰度直方图最大峰值对应的灰度值
    Point firstPeakLoc;
    minMaxLoc(histogram, NULL, NULL, NULL, &firstPeakLoc);
    int firstPeak = firstPeakLoc.x;
    // 寻找灰度直方图第二个峰值对应的灰度值
    Mat measureDists = Mat::zeros(Size(256, 1), CV_32FC1);
    for (int k = 0; k < 256; k++){
        int hist_k = histogram.at<int>(0, k);
        measureDists.at<float>(0, k) = pow(float(k - firstPeak), 2) * hist_k;
    }
    Point secondPeakLoc;
    minMaxLoc(measureDists, NULL, NULL, NULL, &secondPeakLoc);
    int secondPeak = secondPeakLoc.x;
    // 找到两个峰值之间的最小值对应的灰度值，作为阈值
    Point threshLoc;
    int thresh = 0;
    if (firstPeak < secondPeak){    // 第一个峰值在第二个峰值左侧
        minMaxLoc(histogram.colRange(firstPeak, secondPeak), NULL, NULL, &threshLoc, NULL);
        thresh = firstPeak + threshLoc.x + 1;
    } else{                         // 第一个峰值在第二个峰值右侧
        minMaxLoc(histogram.colRange(secondPeak, firstPeak), NULL, NULL, &threshLoc, NULL);
        thresh = secondPeak + threshLoc.x + 1;
    }
    // 阈值分割
    threshold(image, thresh_out, thresh, 255, THRESH_BINARY);
    return thresh;
}



// 编写一个基于直方图技术的阈值分割函数threshTwoPeaks()，并使用
int main(int argc, const char * argv[]) {

    Mat I1 = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp5/pic1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    Mat I2 = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp5/pic2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    imshow("raw1", I1);
    imshow("raw2", I2);

    Mat output;

    threshTwoPeaks(I1, output);
    imshow("ThreshTwoPeaks_Image1", output);

    threshTwoPeaks(I2, output);
    imshow("ThreshTwoPeaks_Image2", output);


    waitKey(0);
    return 0;
}

