#include<iostream>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

void DownSample(Mat &img1, Mat &img2)
{
	int rows = img1.rows / 2;
	int cols = img1.cols / 2;
	int a;
	int b;
	for (a = 0; a < rows; a++) {
		for (b = 0; b < cols; b++) {
			img2.at<uchar>(a, b) = img1.at<uchar>(a * 2, b * 2);
		}
	}
}

void NN(Mat &img1, Mat &img2)
{
	int rows = img2.rows;
	int cols = img2.rows;
	int a;
	int b;
	for (a = 0; a < rows; a++) {
		for (b = 0; b < cols; b++) {
			img2.at<uchar>(a, b) = img1.at<uchar>(cvRound(a / 2), cvRound(b / 2));
		}
	}
}

void BiLinear(Mat &img1, Mat &img2)
{
	int rows = img2.rows;
	int cols = img2.rows;
	int a;
	int b;
	for (a = 0; a < rows - 2; a++) {
		for (b = 0; b < cols - 2; b++) {
			int x = a / 2;
			int y = b / 2;
			float u = (float)a / 2 - (float)x;
			float v = (float)b / 2 - (float)y;
			img2.at<uchar>(a, b) = (uchar)((1 - u)*(1 - v)*(float)(img1.at<uchar>(x, y)) + (1 - u)*v*(float)(img1.at<uchar>(x, y + 1)) + u * (1 - v)*(float)(img1.at<uchar>(x + 1, y)) + u * v*(float)(img1.at<uchar>(x + 1, y + 1)));
		}
	}
}

void BiCubic(Mat &img1, Mat &img2)
{
	int i;
	int j;
	int a;
	int b;
	int rows = img2.rows;
	int cols = img2.cols;
	double coeffA[256];
	double coeffX[16];
	double coeffY[16];
	double temp;
	for (a = 2; a < rows - 4; a++) {
		for (b = 2; b < cols - 4; b++) {
			temp = 0.0f;
			int x = a / 2;
			int y = b / 2;
			for (int s = -1; s <= 2; s++) {
				for (int t = -1; t <= 2; t++) {
					for (int s1 = 0; s1 <= 3; s1++) {
						for (int t1 = 0; t1 <= 3; t1++) {
							coeffA[(s + 1) * 64 + (t + 1) * 16 + s1 * 4 + t1] = pow(s, s1)*pow(t, t1);
						}
					}
				}
			}
			for (i = 0; i < 16; i++)
				coeffY[i] = (double)(img1.at<uchar>(x - 1 + i / 4, y - 1 + i % 4));

			CvMat*   A = cvCreateMat(16, 16, CV_64FC1);
			CvMat*   X = cvCreateMat(16, 1, CV_64FC1);
			CvMat*   B = cvCreateMat(16, 1, CV_64FC1);
			cvSetData(A, coeffA, CV_AUTOSTEP);
			cvSetData(B, coeffY, CV_AUTOSTEP);
			cvSolve(A, B, X, CV_LU);

			for (i = 0; i < 16; i++)
				coeffX[i] = X->data.db[i];

			double u = (float)a / 2 - (float)x;
			double v = (float)b / 2 - (float)y;
			for (i = 0; i <= 3; i++) {
				for (j = 0; j <= 3; j++)
					temp += pow(u, i)*pow(v, j)*coeffX[i * 4 + j];
			}
			img2.at<uchar>(a, b) = (uchar)temp;
		}
	}
}

int main()
{
	Mat img1 = imread("lena.png",CV_8UC1);
	Mat img2(256, 256, CV_8UC1);
	Mat img3(512, 512, CV_8UC1);
	Mat img4(512, 512, CV_8UC1);
	Mat img5(512, 512, CV_8UC1);
	Mat img6(512, 512, CV_8UC1);
	Mat img7(512, 512, CV_8UC1);
	Mat img8(512, 512, CV_8UC1);
	DownSample(img1, img2);
	NN(img2, img3);
	BiLinear(img2, img4);
	BiCubic(img2, img5);
	cvResize((CvArr*)&(IplImage)img2, (CvArr*)&(IplImage)img6, CV_INTER_NN);
	cvResize((CvArr*)&(IplImage)img2, (CvArr*)&(IplImage)img7, CV_INTER_LINEAR);
	cvResize((CvArr*)&(IplImage)img2, (CvArr*)&(IplImage)img8, CV_INTER_CUBIC);
	imshow("lena", img1);
	imshow("DownSample", img2);
	imshow("NN", img3);
	imshow("BiLinear", img4);
	imshow("BiCubic", img5);
	imshow("OpencvNN", img6);
	imshow("OpencvBiLinear", img7);
	imshow("OpencvBiCubic", img8);
	imwrite("DownSample.png", img2);
	imwrite("NN.png", img3);
	imwrite("BiLinear.png", img4);
	imwrite("BiCubic.png", img5);
	imwrite("OpencvNN.png", img6);
	imwrite("OpencvBiLinear.png", img7);
	imwrite("OpencvBiCubic.png", img8);
	waitKey();
	return 0;
}