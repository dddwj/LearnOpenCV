#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 *  1. 对实验图像1执行膨胀处理（2次膨胀，采用矩形结构元）。
 *  要求创建一个可以调节结构元半径r的进度条，比较不同半径时膨胀结果的不同。
 *  参考书 P214
 */

using namespace std;
using namespace cv;

int r = 1; // 结构元半径
int MAX_R = 20; // 设置最大半径
Mat I;
Mat D;

// 回调函数，调节r
void callBack(int, void *){
    // 创建只有垂直方向的矩形结构元
    Mat s = getStructuringElement(MORPH_RECT, Size(2 * r + 1, 2 * r + 1));
    // 膨胀操作
    dilate(I, D, s);
    // 显示膨胀效果
    imshow("dilate", D);
}


// 对实验图像1执行膨胀处理（2次膨胀，采用矩形结构元）。 参考书 P214
int main(int argc, const char * argv[]) {

    I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp6/pic1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }
    imshow("raw", I);

    // 创建显示膨胀效果的窗口
    namedWindow("dilate", WINDOW_AUTOSIZE);
    // 创建调节r的进度条
    createTrackbar("半径", "dilate", &r, MAX_R, callBack);
    callBack(0, 0);

    waitKey(0);
    return 0;
}

