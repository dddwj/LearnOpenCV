#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
void printMat(Mat m){
    for(int r = 0; r < m.rows; r++){
        for(int c = 0; c < m.cols; c++){
            cout << (int)m.at<uchar>(r, c) << ",";
        }
        cout << endl;
    }
};

int main(int argc, const char * argv[]) {
    // insert code here...
    // argv[0] 存放的是完整程序 argv[1]存放命令行参数

    Mat I = imread("/Users/dddwj/ownCloud/mac/备份/学习/1笔记/201902学期/计算机图像处理/LearnOpenCV/exp1/img8.jpg", CV_LOAD_IMAGE_GRAYSCALE); // 按灰度图来读取，I.channels() = 1
    if(!I.data){
        cout << "Can't open image!" << endl;
        return -1;
    }
    imshow("raw", I);

    Mat area = I.rowRange(Range(50,100)).colRange(Range(50,100));       //  或使用 Mat mr = I.row(1);
    for(int r = 0; r < area.rows; r++){       // area直接指向I的地址，修改area就等于修改了I.
        for(int c = 0; c < area.cols; c++){
            area.at<uchar>(r, c) = 0;        // 读灰度图必须用uchar类型
        }
    }

    imshow("modified area", I);
    waitKey(0);
    return 0;

/*
    // 图像金字塔 ref: http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/imgproc/pyramids/pyramids.html

    Mat tmp = I;
    Mat dst = tmp;
    String window_name = "Pyramids Demo";
    /// 创建显示窗口
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );
    imshow( window_name, dst );

    /// 循环
    while( true )
    {
        int c;
        c = waitKey(10);

        if( (char)c == 27 ) // ESC == 27
        { break; }
        if( (char)c == 'u' )
        { pyrUp( tmp, dst, Size( tmp.cols*2, tmp.rows*2 ) );
            printf( "** Zoom In: Image x 2 \n" );
        }
        else if( (char)c == 'd' )
        { pyrDown( tmp, dst, Size( tmp.cols/2, tmp.rows/2 ) );
            printf( "** Zoom Out: Image / 2 \n" );
        }

        imshow( window_name, dst );
        tmp = dst;
    }
    return 0;
*/
}

