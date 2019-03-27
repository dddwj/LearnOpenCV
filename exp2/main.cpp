#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace std;
using namespace cv;

template<typename T>
void printMat(Mat m){
    for(int r = 0; r < m.rows; r++){
        for(int c = 0; c < m.cols; c++){
            cout << (T)m.at<T>(r, c) << ",";
        }
        cout << endl;
    }
};

int main(int argc, const char * argv[]) {
    // insert code here...
    // argv[0] 存放的是完整程序 argv[1]存放命令行参数

    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp2/img.jpg", CV_LOAD_IMAGE_GRAYSCALE); // 按灰度图来读取，I.channels() = 1
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }
    imshow("raw", I);

    // 仿射变换：[法一 warpAffine函数] 等比例放大2倍
    Mat dst1_1;
    Mat dst1_M = (Mat_<float>(2,3) << 2,0,0, 0,2,0);
    warpAffine(I, dst1_1, dst1_M, Size(I.cols*2, I.rows*2)); // Size()内，先'宽' 后'高'

    // 仿射变换：[法二 rezise函数] 等比例放大2倍
    Mat dst1_2;
    resize(I, dst1_2, Size(I.cols*2, I.rows*2), 2, 2);

    // 仿射变换：缩小0.8倍，平移到点(180,400)，绕图像中心逆时针旋转60度
    Mat dst2;
    double dst2_M_rotate_angle = 60./180 * M_PI;
    Mat dst2_M_scale = (Mat_<double>(3,3) << 0.8,0,0,
                                            0,0.8,0,
                                            0,0,1);
    Mat dst2_M_trans = (Mat_<double>(3,3) << 1,0,180,
                                            0,1,400,
                                            0,0,1);
    Mat dst2_M_rotate = (Mat_<double>(3,3) << cos(dst2_M_rotate_angle), sin(dst2_M_rotate_angle), 0,
                                            -sin(dst2_M_rotate_angle), cos(dst2_M_rotate_angle), 0,
                                            0,0,1);
    Mat dst2_M =  dst2_M_scale * dst2_M_trans * dst2_M_rotate;   // 使用OpenCV重载的*运算符，实现点乘
    dst2_M = dst2_M.colRange(0,3).rowRange(0,2);        // 取2*3的仿射变换矩阵M
    warpAffine(I, dst2, dst2_M, Size(I.cols*2, I.rows*2), INTER_LINEAR, BORDER_CONSTANT, 125);


    imshow("dst1_1", dst1_1);
    imshow("dst1_2", dst1_2);
    imshow("dst2", dst2);
    waitKey(0);
    return 0;
}

