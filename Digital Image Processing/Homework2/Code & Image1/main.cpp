#include<iostream>
#include<cstdlib>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

void QuickSort(uchar* list, int low, int high)
{
	int i = low;
	int j = high;
	int temp = list[i];

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

void ImpulseNoise(Mat &img2, int num)
{
	int rows = img2.rows;
	int cols = img2.cols;
	int a;
	int b;
	bool flag = false;
	while (num != 0) {
		a = rand() % rows;
		b = rand() % cols;
		if (flag == false) {
			img2.at<uchar>(a, b) = 0;
			flag = true;
		}
		else {
			img2.at<uchar>(a, b) = 255;
			flag = false;
		}
		num--;
	}
}

void AverageFilter(Mat &img1, Mat &img2)
{
	int rows = img1.rows;
	int cols = img1.cols;
	int a;
	int b;
	for (a = 1; a < rows - 1; a++) {
		for (b = 1; b < cols - 1; b++) {
			img2.at<uchar>(a, b) = (img1.at<uchar>(a - 1, b - 1)
								  + img1.at<uchar>(a, b - 1)
								  + img1.at<uchar>(a + 1, b - 1)
								  + img1.at<uchar>(a - 1, b)
								  + img1.at<uchar>(a, b)
								  + img1.at<uchar>(a + 1, b)
								  + img1.at<uchar>(a - 1, b + 1)
								  + img1.at<uchar>(a, b + 1)
								  + img1.at<uchar>(a + 1, b + 1)) / 9;
		}
	}
}

void MedianFilter(Mat &img1, Mat &img2)
{
	uchar pixellist[9];
	int rows = img1.rows;
	int cols = img1.cols;
	int a;
	int b;
	for (a = 1; a < rows - 1; a++) {
		for (b = 1; b < cols - 1; b++) {
			pixellist[0] = img1.at<uchar>(a - 1, b - 1);
			pixellist[1] = img1.at<uchar>(a, b - 1);
			pixellist[2] = img1.at<uchar>(a + 1, b - 1);
			pixellist[3] = img1.at<uchar>(a - 1, b);
			pixellist[4] = img1.at<uchar>(a, b);
			pixellist[5] = img1.at<uchar>(a + 1, b);
			pixellist[6] = img1.at<uchar>(a - 1, b + 1);
			pixellist[7] = img1.at<uchar>(a, b + 1);
			pixellist[8] = img1.at<uchar>(a + 1, b + 1);
			QuickSort(pixellist, 0, 8);
			img2.at<uchar>(a, b) = pixellist[4];
		}
	}
}

int main() {
	Mat img1 = imread("lena.png", CV_8UC1);
	Mat img2 = imread("lena.png", CV_8UC1);
	Mat img3(512, 512, CV_8UC1);
	Mat img4(512, 512, CV_8UC1);
	Mat img5(512, 512, CV_8UC1);
	ImpulseNoise(img2, 100000);
	AverageFilter(img2, img3);
	MedianFilter(img2, img4);
	MedianFilter(img4, img5);
	imshow("lena", img1);
	imshow("ImpulseNoise", img2);
	imshow("AverageFilter", img3);
	imshow("MedianFilter", img4);
	imshow("DoubleMedianFilter", img5);
	imwrite("ImpulseNoise.png", img2);
	imwrite("AverageFilter.png", img3);
	imwrite("MedianFilter.png", img4);
	imwrite("DoubleMedianFilter.png", img5);
	waitKey();

	return 0;
}