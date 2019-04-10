#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 * 2. 编写一个快速均值平滑函数fastMeanBlur()，需要使用OpenCV的integral()函数
 * 参考书P151
 *
 */

using namespace std;
using namespace cv;

Mat fastMeanBlur(Mat img, Size winSize, int borderType=BORDER_CONSTANT, Scalar value=Scalar(120)){
    // 判断窗口的高、宽是奇数
    int hei = winSize.height;
    int wid = winSize.width;
    CV_Assert(hei % 2 == 1 && wid % 2 == 1);
    // 窗口半径
    int hh = (hei - 1) / 2;
    int ww = (wid - 1) / 2;
    // 窗口的面积
    auto area = float(hei * wid);
    // 边界扩充
    Mat paddImg;
    copyMakeBorder(img, paddImg, hh, hh, ww, ww, borderType, value);    //以[0,255]的value值来填充扩展区域
    // 图像积分
    Mat inte;
    integral(paddImg, inte, CV_32FC1);
    // 输入图像矩阵的高、宽
    int rows = img.rows;
    int cols = img.cols;
    int r = 0, c = 0;
    Mat meanImg = Mat::zeros(img.size(), CV_32FC1);
    for (int h = hh; h < hh+rows; h++){
        for (int w = ww; w < ww+cols; w++){
            float bottomRight = inte.at<float>(h + hh + 1, w + ww + 1);
            float topLeft = inte.at<float>(h - hh, w - ww);
            float topRight = inte.at<float>(h + hh + 1, w - ww);
            float bottomLeft = inte.at<float>(h - hh, w + ww + 1);
            meanImg.at<float>(r, c) = (bottomRight + topLeft - topRight - bottomLeft) / area;
            c++;
        }
        r++;
        c = 0;
    }
    meanImg.convertTo(meanImg, CV_32FC1, 1 / 255.0, 0);     // 注意将数字除以255.0，否则输出的图像为白色。
    return meanImg;
}

// 编写一个快速均值平滑函数fastMeanBlur()，并使用
int main(int argc, const char * argv[]) {

    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp4/pic2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }
    imshow("raw", I);

    Mat O = fastMeanBlur(I, Size(19,19));
    imshow("FastMeanBlur", O);

    waitKey(0);
    return 0;
}

