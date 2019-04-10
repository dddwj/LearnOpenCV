#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 *  3. 编写一个中值平滑函数MedianSmooth(),需要使用OpenCV的sort()函数
 *  参考书P158
 *
 */

using namespace std;
using namespace cv;

Mat medianSmooth(const Mat & I, Size size, int borderType = BORDER_DEFAULT){
    CV_Assert(I.type() == CV_8UC1);
    int H = size.height;
    int W = size.width;
    // 窗口的高、宽均为奇数，一般设置两者是相同的
    CV_Assert(H > 0 && W > 0);
    CV_Assert(H % 2 == 1 && W % 2 ==1);
    // 对原图像矩阵进行边界扩充，省去判断边界的问题
    int h = (H - 1) / 2;
    int w = (W - 1) / 2;
    Mat Ip;
    copyMakeBorder(I, Ip, h, h, w, w, borderType);
    // 输入图像的高、宽
    int rows = I.rows;
    int cols = I.cols;
    // 中值平滑后的输出图像
    Mat medianI(I.size(), CV_8UC1);
    int i = 0, j = 0;
    // 中数的位置
    int index = (H*W - 1) / 2;
    for (int r = h; r < h + rows; r++){
        for (int c = w; c < w + cols; c++){
            // 取当前位置为中心、大小为size的邻域
            Mat region = Ip(Rect(c-w, r-h, W, H)).clone();
            // 将该邻域转换成行矩阵
            region = region.reshape(1, 1);
            // 排序
            cv::sort(region, region, CV_SORT_EVERY_ROW);
            // 取中数
            uchar mValue = region.at<uchar>(0, index);
            medianI.at<uchar>(i, j) = mValue;
            j++;
        }
        i++;
        j = 0;
    }
    return medianI;
}


// 编写一个中值平滑函数MedianSmooth()，并使用
int main(int argc, const char * argv[]) {

    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp4/pic3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }
    imshow("raw", I);

    Mat O = medianSmooth(I, Size(17,17));
    imshow("MedianSmooth", O);


    waitKey(0);
    return 0;
}


