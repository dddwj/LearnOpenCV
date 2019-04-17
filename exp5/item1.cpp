#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 * 1. 使用threshhold()函数，
 * 分别采用THRESH_OTSU、
 *      THRESH_BINARY、
 *      THRESH_TOZERO、
 *      THRESH_TRUNC
 *  四种方法来对实验图像1进行阈值分割，比较分割结果。
 *
 */

using namespace std;
using namespace cv;


// 四种方法来对实验图像1进行阈值分割，参考书P183
int main(int argc, const char * argv[]) {

    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp5/pic1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }
    imshow("raw", I);

    // THRESH_OSTU
    Mat output_otsu;
    threshold(I, output_otsu, 0, 255, THRESH_OTSU);
    imshow("THRESH_OTSU_148", output_otsu);

    // 先利用OSTU自动计算出阈值, 再利用BINARY规则。
    threshold(I, output_otsu, 0, 255, THRESH_OTSU + THRESH_BINARY);
    imshow("THRESH_OTSU_BINARY", output_otsu);

    // THRESH_BINARY
    Mat output_binary;
    threshold(I, output_binary, 148, 255, THRESH_BINARY);
    imshow("THRESH_BINARY", output_binary);

    // THRESH_TOZERO
    Mat output_tozero;
    threshold(I, output_tozero, 148, 255, THRESH_TOZERO);
    imshow("THRESH_TOZERO", output_tozero);

    // THRESH_TRUNC
    Mat output_trunc;
    threshold(I, output_trunc, 148, 255, THRESH_TRUNC);
    imshow("THRESH_TRUNC", output_trunc);


    waitKey(0);
    return 0;
}

