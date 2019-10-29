#include<iostream>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

void GetHistogram(Mat &img, int* histogram)
{
	memset(histogram, 0, sizeof(int) * 256);
	int rows = img.rows;
	int cols = img.cols;
	int a;
	int b;
	for (a = 0; a < rows; a++)
		for (b = 0; b < cols; b++)
			histogram[img.at<uchar>(a, b)]++;
}

void HistogramEqualization(Mat &img, int* histogram)
{
	int rows = img.rows;
	int cols = img.cols;
	int a;
	int b;
	float sum[256];
	memset(sum, 0, sizeof(float) * 256);
	for (a = 0; a < 256; a++) {
		for (b = 0; b <= a; b++) {
			sum[a] += histogram[b];
		}
		sum[a] /= (rows*cols);
	}
	for (a = 0; a < rows; a++)
		for (b = 0; b < cols; b++)
			img.at<uchar>(a, b) = sum[img.at<uchar>(a, b)] * 255;
}

int main()
{
	Mat img1 = imread("lena.png", CV_8UC1);
	Mat img2 = imread("lena.png", CV_8UC1);
	int histogram1[256];
	int histogram2[256];
	GetHistogram(img1, histogram1);
	GetHistogram(img1, histogram2);
	HistogramEqualization(img2, histogram2);
	GetHistogram(img2, histogram2);
	imshow("Before", img1);
	imshow("After", img2);
	for (int i = 0; i < 256; i++)
		printf("histogram1[%d] = %d ,histogram2[%d] = %d.\n", i, histogram1[i], i, histogram2[i]);
	imwrite("After.png", img2);
	waitKey();

	return 0;
}