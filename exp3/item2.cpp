#include "item2.h"
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 * 2.  用伽马变化增强实验图像的对比度
 *
 */

using namespace std;
using namespace cv;

// 用伽马变化增强实验图像的对比度  参考书P113
int main(int argc, const char * argv[]) {

    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp3/pic2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }
    imshow("raw", I);

    // 灰度值归一化, 使得灰度值在[0,1]之间
    Mat fI;
    I.convertTo(fI, CV_64F, 1.0 / 255, 0);    // pow函数要求：元素数据类型为 depth == CV_32F || depth == CV_64F

    // 伽马变换
    double gamma = 0.1;  // 0<gamma<1: ROI变亮； gamma>1: ROI变暗
    Mat Output;
    pow(fI, gamma, Output);
    imshow("gamma", Output);

//    本地保存Output时，需要将灰度值从[0,1]转为[0,255]的CV_8U类型，否则输出图像会变白色（灰度[0,1]）。
//    Output.convertTo(Output, CV_8U, 255, 0);
//    imwrite("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp3/Output.jpg", O);

    waitKey(0);
    return 0;
}

