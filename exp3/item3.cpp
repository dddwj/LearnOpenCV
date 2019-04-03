#include "item3.h"
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 *  3. 用两种方式实现图像直方图全局均衡化：
 *  1）直接调用equalizeHist()函数;
 *  2) 自行编写equalHist()函数实现。
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

Mat equalHist(Mat image){
    CV_Assert(image.type() == CV_8UC1);
    // 灰度图像的高、宽
    int rows = image.rows;
    int cols = image.cols;

    // 第一步：计算图像的灰度直方图
    Mat grayHist = calcGrayHist(image);

    // 第二步：计算累加灰度直方图
    Mat zeroCumuMoment = Mat::zeros(Size(256,1), CV_32SC1);
    for(int p = 0; p < 256; p++){
        if (p == 0)
            zeroCumuMoment.at<int>(0, p) = grayHist.at<int>(0, 0);
        else
            zeroCumuMoment.at<int>(0, p) = zeroCumuMoment.at<int>(0, p-1) + grayHist.at<int>(0, p);
    }

    // 第三步：根据累加直方图得到输入灰度级和输出灰度级之间的映射关系
    Mat output_q = Mat::zeros(Size(256, 1), CV_8UC1);
    float cofficient = 256.0 / (rows * cols);
    for(int p = 0; p < 256; p++){
        float q = cofficient * zeroCumuMoment.at<int>(0, p) - 1;    // 映射公式参考P115
        if (q >= 0)
            output_q.at<uchar>(0, p) = uchar(floor(q)); //floor()向下取整; uchar()截断数据到[0,255]
        else
            output_q.at<uchar>(0, p) = 0;
    }

    // 第四步：得到直方图均衡化后的图像
    Mat equalHistImage = Mat::zeros(image.size(), CV_8UC1);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            int p = image.at<uchar>(r, c);
            equalHistImage.at<uchar>(r, c) = output_q.at<uchar>(0, p);
        }
    }
    return equalHistImage;
}


// 用两种方式实现图像直方图全局均衡化  参考书P117
int main(int argc, const char * argv[]) {

    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp3/pic3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }
    imshow("raw", I);

    // 1）直接调用equalizeHist()函数;
    Mat dst_1;
    equalizeHist(I, dst_1);
    imshow("equalizeHist", dst_1);

    // 2) 自行编写equalHist()函数实现
    Mat dst_2 = equalHist(I);
    imshow("equalHist", dst_2);

    waitKey(0);
    return 0;
}


