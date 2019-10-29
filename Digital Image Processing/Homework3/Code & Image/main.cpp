#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("lena.png", CV_8UC1);
	imshow("origin", image);

	// 获取最佳傅里叶变换尺寸
	int optM = getOptimalDFTSize(image.rows);
	int optN = getOptimalDFTSize(image.cols);

	// 边界扩展
	Mat expandedImage;
	copyMakeBorder(image, expandedImage, 0, optM - image.rows, 0, optM - image.cols, BORDER_CONSTANT, Scalar::all(0));
	
	// 转成浮点图像
	expandedImage.convertTo(expandedImage, CV_32FC1);

	// 使频谱平移到中心
	for (int i = 0; i < expandedImage.rows; i++) {
		for (int j = 0; j < expandedImage.cols; j++) {
			expandedImage.at<float>(i,j) *= (float)pow(-1, i + j);
		}
	}

	// 建立双通道（实部与虚部）图像
	Mat planes[] = { Mat_<float>(expandedImage),Mat::zeros(expandedImage.size(),CV_32FC1) };

	// 合成双通道
	Mat complexImage;
	merge(planes, 2, complexImage);

	// 对双通道图像进行傅里叶变换
	dft(complexImage, complexImage);

	// 定义Gauss变换矩阵
	Mat gaussianBlur(expandedImage.size(), CV_32FC2);
	Mat gaussianSharpen(expandedImage.size(), CV_32FC2);

	float D01 = 64; // 高斯低通滤波器截止频率
	float D02 = 64; // 高斯高通滤波器截止频率
	for (int i = 0; i < expandedImage.rows; i++) {
		for (int j = 0; j < expandedImage.cols; j++) {
			float D = (float)pow(i - expandedImage.rows / 2, 2) + (float)pow(j - expandedImage.cols / 2, 2);
			gaussianBlur.at<Vec2f>(i, j)[0] = expf(-D / (2 * D01*D01));
			gaussianBlur.at<Vec2f>(i, j)[1] = expf(-D / (2 * D01*D01));
			gaussianSharpen.at<Vec2f>(i, j)[0] = 1 - expf(-D / (2 * D02*D02));
			gaussianSharpen.at<Vec2f>(i, j)[1] = 1 - expf(-D / (2 * D02*D02));
		}
	}

	multiply(complexImage, gaussianBlur, gaussianBlur); // 高斯低通滤波模糊图像
	multiply(gaussianBlur, gaussianSharpen, gaussianSharpen); //高斯高通滤波对模糊图像还原

	// 原图像频谱
	split(complexImage, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_MINMAX);
	imshow("spect origin", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_MINMAX);
	imwrite("spect origin.png", planes[0]);

	// 模糊图像频谱
	split(gaussianBlur, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_MINMAX);
	imshow("spect gaussianBlur", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_MINMAX);
	imwrite("spect gaussianBlur.png", planes[0]);

	// 模糊还原后图像频谱
	split(gaussianSharpen, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_MINMAX);
	imshow("spect gaussianSharpen", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_MINMAX);
	imwrite("spect gaussianSharpen.png", planes[0]);

	// 模糊后图像
	idft(gaussianBlur, gaussianBlur);
	split(gaussianBlur, planes);
	magnitude(planes[0], planes[1], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_MINMAX);
	imshow("gaussian Blurred", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_MINMAX);
	imwrite("gaussian Blurred.png", planes[0]);

	// 模糊还原后图像
	idft(gaussianSharpen, gaussianSharpen);
	split(gaussianSharpen, planes);
	magnitude(planes[0], planes[1], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_MINMAX);
	imshow("gaussian Sharpened", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_MINMAX);
	imwrite("gaussian Sharpened.png", planes[0]);

	waitKey();
	return 0;
}