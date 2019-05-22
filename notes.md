# OpenCV学习时的一些注意点

### Mat  访问矩阵某行某列元素 
读取图像，进行一些简单操作

> 实验1.3  

```c++
void readImage(){
	Mat image = imread("path/to/image.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data){
		cout << "Cannot open" << endl;
		return -1;
	}
    namedWindow("raw", WINDOW_AUTOSIZE); // 创建名为raw的窗口
	imshow("raw", image); // 在名为raw的窗口中显示image
}

void printMat(){
	Mat image;
	for (int r = 0; r < image.rows; r++){
		for ( int c = 0; c < image.cols; c++){
			cout << (int)image.at<uchar>(r, c) << ",";
		}
	}
}

void chooseArea(){
	Mat area = image.rowRange(Range(50, 100)).colRange(Range(50, 100));
}

void chooseLine(){
    Mat row = image.row(10); // 取第十行，返回Mat类型
    Mat col = image.col(10); // 取第十列
}
```



### 图像平移、 缩放、旋转
warpAffine( )      getRotation()   resize()

> 实验2 .1

```c++
// 放大2倍
Mat image, dst;
Mat M = (Mat_<float>(2, 3) << 2,0,0, 0,2,0); 
warpAffine(image, dst, M, Size(I.cols*2, I.rows*2));
imshow("dst", dst);

// 缩小为0.8倍
Mat image, dst;
Mat M = (Mat_<float>(3,3) << 4/5,0,0, 0,4/5,0, 0,0,1); 
M = M.colRange(0, 3).rowRange(0, 2);
warpAffine(image, dst, M, Size(I.cols*2, I.rows*2)) // 画布为原图像的2倍
imshow("dst", dst);

// 放大2倍 【resize()】
Mat image, dst;
resize(image, dst, Size(I.cols*2, I.rows*2), fx=2, fy=2); // x方向2倍，y方向2倍

// 旋转【getRotation()】
Mat M = getRotationMatrix2D(Point2f center, double angle, double scale)
// 注：getAffineTransform需要至少三组对应点

// 平移向量(180,400)
Mat M = (Mat_<float>(2, 3) << 1,0,180, 0,1,400); 

// 旋转a=60度逆时针
double a = 60./180 * M_PI;
Mat M = (Mat_<float>(2, 3) << cos(a),sin(a),0,   -sin(a),cos(a),0,  0,0,1 )
```



### 图像平滑  ：高斯平滑、中值平滑、均值平滑

GaussianBlur()
boxFilter()
medianBlur()

> 实验4.1 

```c++
// 高斯平滑
Mat image, dst;
GaussianBlur(image, dst, Size(5,5), sigmaX=3, sigmaY===0, borderType=BORDER_DEFAULT);

// 均值平滑
Mat image, dst;
boxFilter(image, dst, image.type()===0, 
          Size ksize, Point(-1,-1), normalize=true, borderType=BORDER_DEFAULT) 
    // 灰度图的位深image.type()=0
    // Point(-1,-1): 中心点
    
// 中值平滑
Mat image, dst;
medianBlur(image, dst, ksize=5); 
	// ksize: 窗口大小为5*5
```



### 图像访问时注意的点

```c++
Size(int cols, int rows); // 先列 后行
Mat(int rows, int cols); // 先行 后列 
	eg: Mat m = Mat(2, 3, CV_32FC(1)); // 构造2行3列的矩阵
		Mat o = Mat::ones(2, 3, CV_32FC(1)); 	// 2行3列 全1矩阵
		Mat m = (Mat_<int>(2, 3) << 11, 22, 33, 43, 51, 16); // 2行3列

mat.at<uchar>(Point(c, r));
mat.at<uchar>(r, c);


image.convertTo(dst, CV_32FC1, double alpha=1, double beta=0); // 线性变换
```



### 双线性插值



### 直方图正规化(线性变换)

> 灰度级： 0是黑，255是白



### 直方图均值化

对于输入p，总能找到对应的q，进行归一化。



### Sobel、Robert 、Prewitt边缘检测算子

Sobel、Robert 、Prewitt算子的卷积核

注意：水平差分方向上的卷积反应的是垂直方向上的边缘。



### Canny 边缘检测过程

1. 使用Sobel/Prewitt算子与图像卷积得到水平梯度dx和垂直梯度dy，进而得到梯度强度magnitude=sqrt(dx^2+dy^2)
2. 计算梯度方向 angle = arctan2(dy(r,c), dx(r,c))
3. 沿着梯度方向，执行<u>非极大抑制处理</u>，细化边缘强度图。当前点的强度大于左右两个点，认为是极大值，否则抑制极大值、不认为是边缘点。
4. 利于用双阈值执行阈值分割，输出边缘图

```c++
void Canny(image, dst, 
           double threshold1, double threshold2, int kSize, bool L2gradient==false);
```



### 双阈值边缘检测 

1. 边缘强度大于高阈值的那些点作为‘确定边缘点’
2. 边缘强度大于低阈值的那些点作为‘非边缘点’
3. 边缘强度在高低阈值之间的那些点：如果能同构有效路径与高阈值点相连接，则标为边缘点。

（路径上每个点边缘强度要大于低阈值）【首先选定边缘强度大于高阈值点所有确定边缘点，然后在边缘强度大于低阈值的情况下尽可能延长边缘】



### Canny算子相对Sobel的改进

- Sobel、Prewwit等基于卷积方法存在一些缺陷

  - 仅仅利用了边缘的梯度强度，没有利用边缘的梯度方向
  - 最后输出的边缘二值图，只执行简单的阈值处理，输出边缘二值图。如果阈值过大，会损失很多边缘信息；如果阈值过小，会有很多噪声。

- Roberts缺陷

  - 对噪声高度敏感
  - 优势：没有截断

- Canny做了针对性的改进

  （1） 在边缘梯度方向，进行非极大值抑制。

  （2） 双阈值的滞后阈值处理。



### 形态学开运算、闭运算、形态学梯度

> 腐蚀：输出邻域最小值。亮的地方被缩小（/腐蚀）。（用减号）
>
> 膨胀：输出邻域最大值。亮的地方扩张。（用加号）
- 开运算：先腐蚀后膨胀。去掉毛刺。

  > 消除亮度较高的细小区域、在纤细点处分离物体，对于较大物体，可以在不明显改变其面积的情况下平滑其边界。

- 闭运算：先膨胀后腐蚀。补小黑洞。

  > 具有填充白色物体内细小黑色空洞的区域、连接临近物体、同一个结构元、多次迭代处理，也可以在不明显改变其面积的情况下平滑其边界。

- 形态学梯度：求出物体的边缘。边缘线 = 膨胀 - 腐蚀。

- 对比开运算和腐蚀

  > 1. 都能消除高亮部分。
  >
  > 2. 开运算保持物体大小，膨胀会让物体变大。



### 程序1 - 腐蚀

```c++
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

    I = imread("pic1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
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
```



### 程序2 - Sobel边缘检测 

- Sobel()函数的参数意义

```c++
void Sobel(	Mat image, 
           	Mat dst,
          	int ddepth == CV_32FC1,
          	int dx 是否与水平方向Sobel核卷积,
          	int dy 是否与垂直方向Sobel核卷积，dx=0时有效,
          	int ksize == 1,3,5,7... sobel核的尺寸,
          	double scale == 1 比例系数,
          	double delta == 0 平移系数,
          	int borderType == BORDER_DEFAULT
          	)
```

- imshow图像输出的类型转换

```c++
// abs输出
convertScableAbs(image, abs_image);
imshow("image", image);

// 8UC1输出
image.convertTo(image, CV_8UC1);
imshow("image", image);
```

- 代码 

```c++
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){
    Mat image = imread("path/to/image", CV_LOAD_IMAGE_GRAYSCALE);
    if(image.data == NULL){
        cout << "Cannot open!" << endl;
        return -1;
    }
    
    // 水平梯度
    Mat img_gx;
    Sobel(image, img_gx, CV_32FC1, 1, 0, 3); // 水平方向，核的尺寸为3
    Mat abs_img_gx;
    convertScableAbs(img_gx, abs_img_gx);
    imshow("水平梯度", abs_img_gx);
    
    // 垂直梯度
    Mat img_gy;
    Sobel(image, img_gy, CV_32FC1, 0, 1, 3); // 垂直方向，核的尺寸为3
    Mat abs_img_gy;
    convertScableAbs(img_gy, abs_img_gy);
    imshow("垂直梯度", abs_img_gy);
    
    // 边缘强度，采用平方根
    Mat img_gx2, img_gy2, edge;
    pow(img_gx, 2, img_gx2);
    pow(img_gy, 2, img_gy2);
    sqrt(img_gx2 + img_gy2, edge);
    edge.convertTo(edge, CV_8UC1);
    imshow("边缘", edge);
    
    // Sobel算子边缘检测图的反色图
	edge.convertTo(edge, CV_32FC1);
    int r = edge.rows;
    int c = dege.cols;
    Mat edge_inv = Mat(r, c, CV_32FC1);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++){
            edge_inv.at<float>(i, j) = 255 - (int)edge.at<float>(i, j);
        }
	}
	edge_inv.convertTo(edge_inv, CV_8UC1); // 总使用CV_32FC1类型来计算，使用CV_8UC1类型来输出！
	imshow("边缘反色图", edge_inv);

	waitkey(0);
	return 0;
}
```