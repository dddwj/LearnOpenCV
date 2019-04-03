#include "item1.h"
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 * 1. 用两种方式实现图像直方图正规化：
 * 1）直接使用normalize()函数；
 * 2）通过对每个像素点的灰度进行正规化线性变换
 *
 */

using namespace std;
using namespace cv;

template<typename T>
void printMat(Mat m){
    for(int r = 0; r < m.rows; r++){
        for(int c = 0; c < m.cols; c++){
            cout << (double)m.at<T>(r, c) << ",";
        }
        cout << endl;
    }
};

//两种方式实现图像直方图正规化
int main(int argc, const char * argv[]) {

    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp3/pic1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }
    imshow("raw", I);

    // 1) 直接使用normalize()函数
    Mat dst_1;
    normalize(I, dst_1, 255, 0, NORM_MINMAX, CV_8U);
    imshow("normalized dst_1", dst_1);

    // 2) 通过对每个像素点的灰度进行正规化线性变换，使用convertScaleAbs()函数
    Mat dst_2;
    double Imax, Imin;
    minMaxLoc(I, &Imin, &Imax, NULL, NULL);  // 计算矩阵中的最大值、最小值
    double Omin = 0, Omax = 255;
    // 计算 a 和 b
    double a = (Omax - Omin) / (Imax - Imin);
    double b = Omin - a * Imin;
    convertScaleAbs(I, dst_2, a, b);
    imshow("normalized dst_2", dst_2);

    // 补充：通过对每个像素点的灰度进行正规化线性变换，使用对每个像素点的遍历 线性变换计算公式：Output = a * Input + b，详见书P105
    Mat dst_3 = Mat_<uchar>(I.rows,I.cols);   // 注意：图像显示时，必须为uchar数据类型
    for(int r = 0; r < I.rows; r++){
        for(int c = 0; c < I.cols; c++){
            // cout << (double)I.at<uchar>(r,c) << "," ;
            dst_3.at<uchar>(r, c) = a * (double)I.at<uchar>(r,c) + b; // 注意：像素计算时，不能为uchar数据类型
            // cout << (double)dst_3.at<uchar>(r, c) << endl;
        }
    }
    imshow("normalized dst_3", dst_3);
    
    waitKey(0);
    return 0;
}

