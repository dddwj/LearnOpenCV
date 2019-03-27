#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat pI; // 投影变换后的图
Mat I; // 原图
Point2f IPoint, pIPoint;
int i = 0, j = 0;
Point2f src[4]; // 存储原坐标
Point2f dst[4]; // 存储对应变换的坐标

// 通过以下鼠标事件，在原图中取四个坐标
void mouse_I(int event, int x, int y, int flags, void *param){
    switch (event){
        case CV_EVENT_LBUTTONDOWN:
            IPoint = Point2f(x, y);
            break;
        case CV_EVENT_LBUTTONUP:
            src[i] = IPoint;
            circle(I, src[i], 7, Scalar(0), 3); // 标记
            i += 1;
            break;
        default:
            break;
    }
}

// 通过以下鼠标事件，在要输出的画布上取四个对应的坐标
void mouse_pI(int event, int x, int y, int flags, void *param){
    switch (event){
        case CV_EVENT_LBUTTONDOWN:
            pIPoint = Point2f(x, y);
            break;
        case CV_EVENT_LBUTTONUP:
            dst[j] = pIPoint;
            circle(pI, dst[j], 7, Scalar(0), 3); // 标记
            j += 1;
            break;
        default:
            break;
    }
}


int main(int argc, const char * argv[]) {
    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp2/perspective1.jpg", CV_LOAD_IMAGE_GRAYSCALE); // 按灰度图来读取，I.channels() = 1
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }

    // 输出图像
    pI = 255 * Mat::ones(I.size(), CV_8UC1);
    // 在原图窗口上，定义鼠标事件
    namedWindow("I", WINDOW_AUTOSIZE);
    setMouseCallback("I", mouse_I, NULL);

    // 在输出窗口上，定义鼠标事件
    namedWindow("pI", WINDOW_AUTOSIZE);
    setMouseCallback("pI", mouse_pI, NULL);

    // 等待用户点击
    imshow("I", I);
    imshow("pI", pI);
    while(!(i > 3 && j > 3)){
        imshow("I", I);
        imshow("pI", pI);
        if (waitKey(50) == 'q')
            break;
    }
    imshow("I", I);
    imshow("pI", pI);

    // 移除鼠标事件
    setMouseCallback("I", NULL, NULL);
    setMouseCallback("pI", NULL, NULL);

    // 计算投影变换矩阵
    Mat p = getPerspectiveTransform(src, dst);

    // 投影变换
    Mat result;
    warpPerspective(I, result, p, pI.size());
    imshow("after projection", result);

    waitKey(0);
    return 0;
}