#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

float sigma = 0.002f; // 高斯白噪声方差
float K = 0.002f; // 维纳滤波系数

// 利用Box - Muller算法产生高斯随机数
float GenerateGaussianNoise()
{
	static bool flag = false;
	static float rand1, rand2;

	if (flag)
	{
		flag = false;
		return sqrt(-2.0f * log(rand1)) * cos(2.0f * (float)CV_PI * rand2);
	}

	flag = true;

	rand1 = rand() / ((float)RAND_MAX);
	rand2 = rand() / ((float)RAND_MAX);
	if (rand1 < 1e-10)
		rand1 = (float)1e-10;
	if (rand2 < 1e-10)
		rand2 = (float)1e-10;

	return sqrt(-2.0f * log(rand1)) * sin(2.0f * (float)CV_PI * rand2);
}

void AddGaussianNoise(Mat& Image)
{
	for (int i = 0; i < Image.rows; ++i) {
		for (int j = 0; j < Image.cols; ++j) {
			float value = Image.at<float>(i, j) + GenerateGaussianNoise() * sigma;
			if (value > 1.0f)
				value = 1.0f;
			if (value < 0.0f)
				value = 0.0f;
			Image.at<float>(i, j) = value;
		}
	}
}

int main()
{
	Mat image = imread("原图像.png", CV_8UC1);

	// 获取最佳傅里叶变换尺寸
	int optM = getOptimalDFTSize(image.rows);
	int optN = getOptimalDFTSize(image.cols);
	
	// 边界扩展
	Mat expandedImage;
	copyMakeBorder(image, expandedImage, 0, optM - image.rows, 0, optM - image.cols, BORDER_CONSTANT, Scalar::all(0));

	// 转成浮点图像
	expandedImage.convertTo(expandedImage, CV_32FC1);
	normalize(expandedImage, expandedImage, 1, 0, CV_C);
	imshow("原图像", expandedImage);

	// 使频谱平移到中心
	for (int i = 0; i < expandedImage.rows; i++) {
		for (int j = 0; j < expandedImage.cols; j++) {
			expandedImage.at<float>(i, j) *= (float)pow(-1, i + j);
		}
	}

	// 建立双通道（实部与虚部）图像
	Mat planes[] = { Mat_<float>(expandedImage),Mat::zeros(expandedImage.size(),CV_32FC1) };

	// 合成双通道
	Mat complexImage;
	merge(planes, 2, complexImage);

	// 对双通道图像进行傅里叶变换
	dft(complexImage, complexImage);

	// 定义运动模糊变换矩阵
	Mat Motion(expandedImage.size(), CV_32FC2);
	Mat DeMotion(expandedImage.size(), CV_32FC2);

	float a = 0.03f;
	float b = 0.03f;
	float T = 1.0f;

	float CutOffBlur = 0.01f;
	float CutOffDeblur = 0.01f;
	for (int i = 0; i < expandedImage.rows; i++) {
		for (int j = 0; j < expandedImage.cols; j++) {
			float temp = (float)CV_PI * ((i - expandedImage.rows / 2) * a + (j - expandedImage.cols / 2) * b);
			if (temp == 0) {
				Motion.at<Vec2f>(i, j)[0] = T;
				Motion.at<Vec2f>(i, j)[1] = T;
				DeMotion.at<Vec2f>(i, j)[0] = T;
				DeMotion.at<Vec2f>(i, j)[1] = T;
			}
			else {
				Motion.at<Vec2f>(i, j)[0] = T / temp * (float)sin(temp)*(float)cos(temp);
				Motion.at<Vec2f>(i, j)[1] = T / temp * (float)sin(temp)*(float)cos(temp);
				DeMotion.at<Vec2f>(i, j)[0] = T / temp * (float)sin(temp)*(float)cos(temp);
				DeMotion.at<Vec2f>(i, j)[1] = T / temp * (float)sin(temp)*(float)cos(temp);
				// 滤波阈值
				if (Motion.at<Vec2f>(i, j)[0] < CutOffBlur) {
					Motion.at<Vec2f>(i, j)[0] = CutOffBlur;
					Motion.at<Vec2f>(i, j)[1] = CutOffBlur;
				}
				// 滤波阈值
				if (DeMotion.at<Vec2f>(i, j)[0] < CutOffDeblur) {
					DeMotion.at<Vec2f>(i, j)[0] = CutOffDeblur;
					DeMotion.at<Vec2f>(i, j)[1] = CutOffDeblur;
				}
			}
		}
	}

	Mat Blurred(expandedImage.size(), CV_32FC2);  // 模糊频谱
	Mat BlurredWithNoise(expandedImage.size(), CV_32FC2); // 模糊带噪声频谱
	Mat BlurredImage(expandedImage.size(), CV_32FC1); // 模糊图像
	Mat BlurredWithNoiseImage(expandedImage.size(), CV_32FC1); // 模糊带噪声图像
	Mat Deblurred(expandedImage.size(), CV_32FC2);  // 模糊逆滤波频谱
	Mat DeblurredWithNoise(expandedImage.size(), CV_32FC2); // 模糊带噪声逆滤波频谱
	Mat DeblurredImage(expandedImage.size(), CV_32FC1); // 模糊逆滤波图像
	Mat DeblurredWithNoiseImage(expandedImage.size(), CV_32FC1); // 模糊带噪声逆滤波图像
	Mat DeblurredWiener(expandedImage.size(), CV_32FC2);  // 模糊维纳滤波频谱
	Mat DeblurredWienerWithNoise(expandedImage.size(), CV_32FC2); // 模糊带噪声维纳滤波频谱
	Mat DeblurredWienerImage(expandedImage.size(), CV_32FC1); // 模糊维纳滤波图像
	Mat DeblurredWienerWithNoiseImage(expandedImage.size(), CV_32FC1); // 模糊带噪声维纳滤波图像

	split(complexImage, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_C);
	imshow("原图像频谱", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_C);
	imwrite("原图像频谱.png", planes[0]);

	multiply(complexImage, Motion, Blurred); // 加入运动模糊
	
	split(Blurred, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_C);
	imshow("模糊图像频谱", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_C);
	imwrite("模糊图像频谱.png", planes[0]);

	divide(Blurred, DeMotion, Deblurred); // 解运动模糊

	split(Deblurred, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_C);
	imshow("直接解模糊图像频谱", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_C);
	imwrite("直接解模糊图像频谱.png", planes[0]);

	idft(Deblurred, Deblurred);
	split(Deblurred, planes);
	magnitude(planes[0], planes[1], DeblurredImage);
	normalize(DeblurredImage, DeblurredImage, 1, 0, CV_C);
	imshow("直接解模糊图像", DeblurredImage);
	normalize(DeblurredImage, DeblurredImage, 255, 0, CV_C);
	imwrite("直接解模糊图像.png", DeblurredImage);

	// 加噪声
	idft(Blurred, Blurred);
	split(Blurred, planes);
	magnitude(planes[0], planes[1], BlurredImage);
	normalize(BlurredImage, BlurredImage, 1, 0, CV_C);
	imshow("模糊图像", BlurredImage);
	BlurredImage.copyTo(BlurredWithNoiseImage);
	AddGaussianNoise(BlurredWithNoiseImage);
	imshow("带噪声模糊图像", BlurredWithNoiseImage);
	normalize(BlurredImage, BlurredImage, 255, 0, CV_C);
	imwrite("模糊图像.png", BlurredImage);
	normalize(BlurredWithNoiseImage, BlurredWithNoiseImage, 255, 0, CV_C);
	imwrite("带噪声模糊图像.png", BlurredWithNoiseImage);

	for (int i = 0; i < BlurredWithNoiseImage.rows; i++) {
		for (int j = 0; j < BlurredWithNoiseImage.cols; j++) {
			BlurredWithNoiseImage.at<float>(i, j) *= (float)pow(-1, i + j);
		}
	}

	BlurredWithNoiseImage.copyTo(planes[0]);
	planes[1] = Mat::zeros(BlurredWithNoiseImage.size(), CV_32FC1);

	merge(planes, 2, BlurredWithNoise);
	dft(BlurredWithNoise, BlurredWithNoise);

	split(BlurredWithNoise, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_C);
	imshow("带噪声模糊图像频谱", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_C);
	imwrite("带噪声模糊图像频谱.png", planes[0]);
	
	divide(BlurredWithNoise, DeMotion, DeblurredWithNoise);

	split(DeblurredWithNoise, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_C);
	imshow("解带噪声模糊图像频谱", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_C);
	imwrite("解带噪声模糊图像频谱.png", planes[0]);

	idft(DeblurredWithNoise, DeblurredWithNoise);
	split(DeblurredWithNoise, planes);
	magnitude(planes[0], planes[1], DeblurredWithNoiseImage);
	normalize(DeblurredWithNoiseImage, DeblurredWithNoiseImage, 1, 0, CV_C);
	imshow("解带噪声模糊图像", DeblurredWithNoiseImage);
	normalize(DeblurredWithNoiseImage, DeblurredWithNoiseImage, 255, 0, CV_C);
	imwrite("解带噪声模糊图像.png", DeblurredWithNoiseImage);

	dft(Blurred, Blurred);
	// 维纳滤波
	Mat temp(expandedImage.size(), CV_32FC2);
	Mat MagTransform(expandedImage.size(), CV_32FC2);
	for (int i = 0; i < MagTransform.rows; i++) {
		for (int j = 0; j < MagTransform.cols; j++) {
			MagTransform.at<Vec2f>(i, j)[0] = ((pow(Motion.at<Vec2f>(i, j)[0], 2) + pow(Motion.at<Vec2f>(i, j)[1], 2)) / (pow(Motion.at<Vec2f>(i, j)[0], 2) + pow(Motion.at<Vec2f>(i, j)[1], 2) + K));
			MagTransform.at<Vec2f>(i, j)[1] = ((pow(Motion.at<Vec2f>(i, j)[0], 2) + pow(Motion.at<Vec2f>(i, j)[1], 2)) / (pow(Motion.at<Vec2f>(i, j)[0], 2) + pow(Motion.at<Vec2f>(i, j)[1], 2) + K));
		}
	}
	divide(Blurred, Motion, temp);
	multiply(temp, MagTransform, DeblurredWiener);
	divide(BlurredWithNoise, Motion, temp);
	multiply(temp, MagTransform, DeblurredWienerWithNoise);

	split(DeblurredWiener, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_C);
	imshow("维纳滤波解模糊图像频谱", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_C);
	imwrite("维纳滤波解模糊图像频谱.png", planes[0]);

	idft(DeblurredWiener, DeblurredWiener);
	split(DeblurredWiener, planes);
	magnitude(planes[0], planes[1], DeblurredWienerImage);
	normalize(DeblurredWienerImage, DeblurredWienerImage, 1, 0, CV_C);
	imshow("维纳滤波解模糊图像", DeblurredWienerImage);
	normalize(DeblurredWienerImage, DeblurredWienerImage, 255, 0, CV_C);
	imwrite("维纳滤波解模糊图像.png", DeblurredWienerImage);

	split(DeblurredWienerWithNoise, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] += Scalar::all(1);
	log(planes[0], planes[0]);
	normalize(planes[0], planes[0], 1, 0, CV_C);
	imshow("维纳滤波解带噪声模糊图像频谱", planes[0]);
	normalize(planes[0], planes[0], 255, 0, CV_C);
	imwrite("维纳滤波解带噪声模糊图像频谱.png", planes[0]);

	idft(DeblurredWienerWithNoise, DeblurredWienerWithNoise);
	split(DeblurredWienerWithNoise, planes);
	magnitude(planes[0], planes[1], DeblurredWienerWithNoiseImage);
	normalize(DeblurredWienerWithNoiseImage, DeblurredWienerWithNoiseImage, 1, 0, CV_C);
	imshow("维纳滤波解带噪声模糊图像", DeblurredWienerWithNoiseImage);
	normalize(DeblurredWienerWithNoiseImage, DeblurredWienerWithNoiseImage, 255, 0, CV_C);
	imwrite("维纳滤波解带噪声模糊图像.png", DeblurredWienerWithNoiseImage);

	waitKey();
	return 0;
}