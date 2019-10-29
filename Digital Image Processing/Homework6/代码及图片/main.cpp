#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

// 二值图像转化
void BinaryCheck(Mat& image, Mat& dst)
{
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) > 127) {
				dst.at<uchar>(i, j) = 255;
			}
			else {
				dst.at<uchar>(i, j) = 0;
			}
		}
	}
}

// 腐蚀操作
void Erosion(Mat& image, Mat& eroded)
{
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (i == 0 || j == 0 || i == image.rows - 1 || j == image.cols - 1) {
				eroded.at<uchar>(i, j) = image.at<uchar>(i, j);
			}
			else {
				eroded.at<uchar>(i, j) = min({ image.at<uchar>(i - 1,j - 1),image.at<uchar>(i - 1,j), image.at<uchar>(i - 1,j + 1), image.at<uchar>(i,j - 1), image.at<uchar>(i,j), image.at<uchar>(i,j + 1), image.at<uchar>(i + 1,j - 1), image.at<uchar>(i + 1,j), image.at<uchar>(i + 1,j + 1) });
			}
		}
	}
}

// 膨胀操作
void Dilation(Mat& image, Mat& dilated)
{
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (i == 0 || j == 0 || i == image.rows - 1 || j == image.cols - 1) {
				dilated.at<uchar>(i, j) = image.at<uchar>(i, j);
			}
			else {
				dilated.at<uchar>(i, j) = max({ image.at<uchar>(i - 1,j - 1),image.at<uchar>(i - 1,j), image.at<uchar>(i - 1,j + 1), image.at<uchar>(i,j - 1), image.at<uchar>(i,j), image.at<uchar>(i,j + 1), image.at<uchar>(i + 1,j - 1), image.at<uchar>(i + 1,j), image.at<uchar>(i + 1,j + 1) });
			}
		}
	}
}

// 图像相减
void Difference(Mat& image1, Mat& image2, Mat& dst)
{
	for (int i = 0; i < image1.rows; i++) {
		for (int j = 0; j < image1.cols; j++) {
			dst.at<uchar>(i, j) = max({ 0, image1.at<uchar>(i,j) - image2.at<uchar>(i,j) });
		}
	}
}

// 二值图像提取边界 -> 3 * 3 正方形模板
void BorderCut(Mat& image, Mat& dst)
{
	Mat binary = Mat(image.size(), CV_8UC1);
	Mat eroded = Mat(image.size(), CV_8UC1);
	BinaryCheck(image, binary);
	Erosion(binary, eroded);
	Difference(binary, eroded, dst);
}

// 灰度图像求形态学梯度 -> 3 * 3 正方形模板
void MorphologicalGradient(Mat& image, Mat& dst)
{
	Mat dilated = Mat(image.size(), CV_8UC1);
	Mat eroded = Mat(image.size(), CV_8UC1);
	Dilation(image, dilated);
	Erosion(image, eroded);
	Difference(dilated, eroded, dst);
}

int main()
{
	Mat image = imread("binary.png", CV_8UC1);
	Mat lena = imread("lena.png", CV_8UC1);
	Mat dst1 = Mat(image.size(), CV_8UC1);
	Mat dst2 = Mat(lena.size(), CV_8UC1);

	BorderCut(image, dst1);
	MorphologicalGradient(lena, dst2);

	imshow("origin", image);
	imshow("bordercut", dst1);
	imshow("lena", lena);
	imshow("gradient", dst2);

	imwrite("bordercut.png", dst1);
	imwrite("gradient.png", dst2);

	waitKey();
	return 0;
}