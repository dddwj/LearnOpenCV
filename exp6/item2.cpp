#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 * 2. 利用OpenCV提供的Sobel()函数（梯度算子尺寸是3×3），
 * 输出实验图像3的水平梯度图、
 *              垂直梯度图、
 *              梯度强度图（边缘强度以平方根形式来衡量）以及
 *              梯度强度图的反色图。
 *  参考Chap8 PPT
 */

using namespace std;
using namespace cv;

// 输出实验图像3的水平梯度图、垂直梯度图、梯度强度图、梯度强度图的反色图

int main(int argc, const char * argv[]) {

    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp6/pic2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    imshow("raw", I);

    // 水平梯度图
    Mat O_gx, O_gx_abs;
    Sobel(I, O_gx, CV_32FC1, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(O_gx, O_gx_abs);        // 取绝对值，从而转为CV_8UC1
    imshow("O_horizonal", O_gx_abs);
//    O_gx.convertTo(O_gx_abs2, CV_8UC1);   // 使用这种方式转换会导致输出错误。但是，从文档上看这种方式与"取绝对值"方法效果一致，待研究源码。
//    imshow("O_horizonal2", O_gx_abs2);

    // 垂直梯度图
    Mat O_gy, O_gy_abs;
    Sobel(I, O_gy, CV_32FC1, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(O_gy, O_gy_abs);
    imshow("O_vertical", O_gy_abs);

    // 梯度强度图
    Mat O_gx1, O_gy1, O_edge;
    pow(O_gx, 2, O_gx1);
    pow(O_gy, 2, O_gy1);
    sqrt(O_gx1 + O_gy1, O_edge); // 平方根形式衡量边缘强度
    O_edge.convertTo(O_edge, CV_8UC1);
    imshow("O_edge", O_edge);

    // 梯度强度图的反色图
    O_edge.convertTo(O_edge, CV_32FC1);     // 注意：总是要使用CV_32FC1类型来计算，使用CV_8UC1类型来输出！
    Mat O_edge_inv = Mat(O_edge.rows, O_edge.cols, CV_32FC1);
    for (int i = 0; i < O_edge.rows; i++){
        for (int j = 0; j < O_edge.cols; j++){
            O_edge_inv.at<float>(i, j) = 255 - (int)O_edge.at<float>(i, j);
        }
    }
    O_edge_inv.convertTo(O_edge_inv, CV_8UC1);
    imshow("O_edge_inv", O_edge_inv);

    waitKey(0);
    return 0;
}

