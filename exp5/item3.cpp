#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 *  3. 编写一个OTSU阈值分割函数otsu()，并用实验图像1和实验图像2来验证分割函数。
 *  参考书P197
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

int otsu(const Mat & image, Mat &OtsuThreshImage){
    // 计算灰度直方图
    Mat histogram = calcGrayHist(image);
    // 归一化灰度直方图
    Mat normHist;
    histogram.convertTo(normHist, CV_32FC1, 1.0 / (image.rows * image.cols), 0.0);
    // 计算累加直方图（零阶累积矩）和一阶累积矩
    Mat zeroCumuMoment = Mat::zeros(Size(256, 1), CV_32FC1);
    Mat oneCumuMoment = Mat::zeros(Size(256, 1), CV_32FC1);
    for (int i = 0; i < 256; i++){
        if (i == 0){
            zeroCumuMoment.at<float>(0, i) = normHist.at<float>(0, i);
            oneCumuMoment.at<float>(0 ,i) = i * normHist.at<float>(0, i);
        } else{
            zeroCumuMoment.at<float>(0, i) = zeroCumuMoment.at<float>(0, i-1) + normHist.at<float>(0, i);
            oneCumuMoment.at<float>(0, i) = oneCumuMoment.at<float>(0, i-1) + i * normHist.at<float>(0, i);
        }
    }
    // 计算类间方差
    Mat variance = Mat::zeros(Size(256, 1), CV_32FC1);
    // 总平均值
    float mean = oneCumuMoment.at<float>(0, 255);
    for ( int i = 0; i < 255; i++){
        if (zeroCumuMoment.at<float>(0, i) == 0 || zeroCumuMoment.at<float>(0, i) == 1)
            variance.at<float>(0, i) = 0;
        else {
            float coefficient = zeroCumuMoment.at<float>(0, i) * (1.0 - zeroCumuMoment.at<float>(0, i));
            variance.at<float>(0, i) = pow(mean*zeroCumuMoment.at<float>(0, i) - oneCumuMoment.at<float>(0, i), 2.0) / coefficient;
        }
    }
    // 找到阈值
    Point maxLoc;
    minMaxLoc(variance,NULL, NULL, NULL, &maxLoc);
    int otsuThresh = maxLoc.x;
    // 阈值处理
    threshold(image, OtsuThreshImage, otsuThresh, 255, THRESH_BINARY);

    return otsuThresh;
}


// 编写一个OTSU阈值分割函数otsu()，并使用
int main(int argc, const char * argv[]) {

    Mat I1 = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp5/pic1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    imshow("raw_image1", I1);

    Mat I2 = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp5/pic2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    imshow("raw_image2", I2);

    Mat output;
    otsu(I1, output);
    imshow("OTSU_image1", output);
    otsu(I2, output);
    imshow("OTSU_image2",output);


    waitKey(0);
    return 0;
}


