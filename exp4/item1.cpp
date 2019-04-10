#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
/*
 * 1. 分别用高斯平滑（5×5,sigma=3）、
 *         均值平滑（3×3）、
 *         中值平滑（7×7) 、
 *         双边平滑（5×5，sigmaColor=2.5）
 *   处理实验图像1，比较平滑输出图像
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

// 实现图像平滑
int main(int argc, const char * argv[]) {

    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp4/pic1.png", CV_LOAD_IMAGE_GRAYSCALE);
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }
    imshow("raw", I);

    // 使用 高斯平滑（5×5,sigma=3), 书P146
    Mat O_Gaussian;
    GaussianBlur(I, O_Gaussian, Size(5, 5), 3, 0, BORDER_DEFAULT);
    imshow("Gaussian", O_Gaussian);

    // 使用 均值平滑 (3×3), 书P153
    Mat O_Mean, O_Mean2;
    boxFilter(I, O_Mean, I.type(), Size(3,3), Point(-1,-1), true, BORDER_DEFAULT);  // 灰度图的'位深' = I.type() = 0;   Point(-1,-1)表示图像中心点.
    blur(I, O_Mean2, Size(3,3), Point(-1,-1), BORDER_DEFAULT); // blur(), boxFilter()函数任选一个即可
    imshow("Mean", O_Mean);
    imshow("Mean2", O_Mean2);

    // 使用 中值平滑（7×7), 书P160
    Mat O_Median;
    medianBlur(I, O_Median, 7);
    imshow("Median", O_Median);

    // 使用 双边平滑（5×5，sigmaColor=2.5), 书P167
    Mat O_bilateral;
    bilateralFilter(I, O_bilateral, 5, 2.5, 0, BORDER_DEFAULT);
    imshow("Bilateral", O_bilateral);
    
    waitKey(0);
    return 0;
}

