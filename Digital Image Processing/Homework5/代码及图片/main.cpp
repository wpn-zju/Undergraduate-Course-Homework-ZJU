#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

// 高斯白噪声标准差
float sigma = 0.1f;

// 椒盐噪声点数
int number = 100000;

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

void QuickSort(float* list, int low, int high)
{
	int i = low;
	int j = high;
	float temp = list[i];

	if (low < high) {
		while (i < j) {
			while ((list[j] >= temp) && (i < j))
				j--;
			list[i] = list[j];
			while ((list[i] <= temp) && (i < j))
				i++;
			list[j] = list[i];
		}
		list[i] = temp;
		QuickSort(list, low, i - 1);
		QuickSort(list, j + 1, high);
	}
	else
		return;
}

void ImpulseNoise(Mat &image, int num)
{
	int a;
	int b;
	bool flag = false;
	while (num != 0) {
		a = rand() % image.rows;
		b = rand() % image.cols;
		if (flag == false) {
			image.at<float>(a, b) = 0.0f;
			flag = true;
		}
		else {
			image.at<float>(a, b) = 1.0f;
			flag = false;
		}
		num--;
	}
}

void MedianFilter(Mat &image1, Mat &image2)
{
	float pixellist[9];
	int rows = image1.rows;
	int cols = image1.cols;
	int a;
	int b;
	for (a = 1; a < rows - 1; a++) {
		for (b = 1; b < cols - 1; b++) {
			pixellist[0] = image1.at<float>(a - 1, b - 1);
			pixellist[1] = image1.at<float>(a, b - 1);
			pixellist[2] = image1.at<float>(a + 1, b - 1);
			pixellist[3] = image1.at<float>(a - 1, b);
			pixellist[4] = image1.at<float>(a, b);
			pixellist[5] = image1.at<float>(a + 1, b);
			pixellist[6] = image1.at<float>(a - 1, b + 1);
			pixellist[7] = image1.at<float>(a, b + 1);
			pixellist[8] = image1.at<float>(a + 1, b + 1);
			QuickSort(pixellist, 0, 8);
			image2.at<float>(a, b) = pixellist[4];
		}
	}
}

void AverageFilter(Mat &Image1, Mat &Image2)
{
	for (int i = 1; i < Image1.rows - 1; i++) {
		for (int j = 1; j < Image1.cols - 1; j++) {
			Image2.at<Vec3f>(i, j) = 
				 (Image1.at<Vec3f>(i - 1, j - 1)
				+ Image1.at<Vec3f>(i, j - 1)
				+ Image1.at<Vec3f>(i + 1, j - 1)
				+ Image1.at<Vec3f>(i - 1, j)
				+ Image1.at<Vec3f>(i, j)
				+ Image1.at<Vec3f>(i + 1, j)
				+ Image1.at<Vec3f>(i - 1, j + 1)
				+ Image1.at<Vec3f>(i, j + 1)
				+ Image1.at<Vec3f>(i + 1, j + 1)) / 9;
		}
	}
}

int main()
{
	Mat image = imread("lena.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat channel[4]; // RGBA

	// 分离RGBA平面
	split(image, channel);

	// 转换图像类型
	channel[0].convertTo(channel[0], CV_32FC1);
	channel[1].convertTo(channel[1], CV_32FC1);
	channel[2].convertTo(channel[2], CV_32FC1);

	// 归一化到[0,1]浮点数
	normalize(channel[0], channel[0], 1, 0, CV_C);
	normalize(channel[1], channel[1], 1, 0, CV_C);
	normalize(channel[2], channel[2], 1, 0, CV_C);

	imshow("Original", image);

	// 分通道加入高斯白噪声
	AddGaussianNoise(channel[0]);
	AddGaussianNoise(channel[1]);
	AddGaussianNoise(channel[2]);

	// 合并RGB通道
	merge(channel, 3, image);
	imshow("AddGaussianNoise", image);
	normalize(image, image, 255, 0, CV_C);
	imwrite("GaussianNoiseImage.png", image);
	normalize(image, image, 1, 0, CV_C);

	// 均值滤波
	AverageFilter(image, image);
	imshow("AverageFiltered", image);
	normalize(image, image, 255, 0, CV_C);
	imwrite("AverageFiltered.png", image);
	normalize(image, image, 1, 0, CV_C);

	// 再次均值滤波
	AverageFilter(image, image);
	imshow("DoubleAverageFiltered", image);
	normalize(image, image, 255, 0, CV_C);
	imwrite("DoubleAverageFiltered.png", image);
	normalize(image, image, 1, 0, CV_C);

	// 分离RGBA平面
	split(image, channel);

	// 转换图像类型
	channel[0].convertTo(channel[0], CV_32FC1);
	channel[1].convertTo(channel[1], CV_32FC1);
	channel[2].convertTo(channel[2], CV_32FC1);

	// 归一化到[0,1]浮点数
	normalize(channel[0], channel[0], 1, 0, CV_C);
	normalize(channel[1], channel[1], 1, 0, CV_C);
	normalize(channel[2], channel[2], 1, 0, CV_C);

	// 分通道加入高斯白噪声
	ImpulseNoise(channel[0], number);
	ImpulseNoise(channel[1], number);
	ImpulseNoise(channel[2], number);

	// 合并RGB通道
	merge(channel, 3, image);
	imshow("AddSalt-PepperNoise", image);
	normalize(image, image, 255, 0, CV_C);
	imwrite("Salt-PepperNoiseImage.png", image);
	normalize(image, image, 1, 0, CV_C);

	// 均值滤波
	MedianFilter(channel[0], channel[0]);
	MedianFilter(channel[1], channel[1]);
	MedianFilter(channel[2], channel[2]);
	merge(channel, 3, image);

	imshow("MedianFiltered", image);
	normalize(image, image, 255, 0, CV_C);
	imwrite("MedianFiltered.png", image);
	normalize(image, image, 1, 0, CV_C);

	// 再次均值滤波
	MedianFilter(channel[0], channel[0]);
	MedianFilter(channel[1], channel[1]);
	MedianFilter(channel[2], channel[2]);
	merge(channel, 3, image);

	imshow("DoubleMedianFiltered", image);
	normalize(image, image, 255, 0, CV_C);
	imwrite("DoubleMedianFiltered.png", image);
	normalize(image, image, 1, 0, CV_C);

	waitKey();
	return 0;
}