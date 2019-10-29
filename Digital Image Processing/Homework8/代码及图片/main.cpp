#include <iostream>
#include <fstream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;

vector<Point2i> borderPointList;
vector<int> linkedCode;

Point2i GetStartPoint(Mat& image)
{
	for (int i = 0; i < image.rows; ++i)
		for (int j = 0; j < image.cols; ++j)
			if (image.at<uchar>(i, j) == 255)
				return Point2i(i, j);
	return Point2i(0, 0); // error exit
}

Point2i Detect(Mat& image, Point2i lastPointb, Point2i lastPointc)
{
	int dx = lastPointb.x - lastPointc.x;
	int dy = lastPointb.y - lastPointc.y;

	if (dx == -1 && dy == -1)
	{
		linkedCode.push_back(3);
		if (image.at<uchar>(lastPointb.x + 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y + 1);
	}
	if (dx == -1 && dy == 0)
	{
		linkedCode.push_back(2);
		if (image.at<uchar>(lastPointb.x + 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y + 1);
	}
	if (dx == -1 && dy == 1)
	{
		linkedCode.push_back(1);
		if (image.at<uchar>(lastPointb.x, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y);
	}

	if (dx == 0 && dy == 1)
	{
		linkedCode.push_back(0);
		if (image.at<uchar>(lastPointb.x - 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y - 1);
	}
	if (dx == 1 && dy == 1)
	{
		linkedCode.push_back(7);
		if (image.at<uchar>(lastPointb.x - 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y - 1);
	}
	if (dx == 1 && dy == 0)
	{
		linkedCode.push_back(6);
		if (image.at<uchar>(lastPointb.x - 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y - 1);
	}
	if (dx == 1 && dy == -1)
	{
		linkedCode.push_back(5);
		if (image.at<uchar>(lastPointb.x, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y);
	}
	if (dx == 0 && dy == -1)
	{
		linkedCode.push_back(4);
		if (image.at<uchar>(lastPointb.x + 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y + 1);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x + 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x + 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y - 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y - 1);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y);
		else if (image.at<uchar>(lastPointb.x - 1, lastPointb.y + 1) == 255)
			return Point2i(lastPointb.x - 1, lastPointb.y + 1);
	}

	// error exit
	return Point2i(lastPointb.x, lastPointb.y);
}

void BorderDetect(Mat& image)
{
	Point2i startPoint = GetStartPoint(image);
	Point2i newPoint = startPoint;
	Point2i lastPoint;
	lastPoint = Point2i(newPoint.x, newPoint.y - 1);

	while (1)
	{
		borderPointList.push_back(newPoint);
		Point2i temp = Detect(image, newPoint, lastPoint);
		if (temp == startPoint || temp == newPoint)
			break;
		lastPoint = newPoint;
		newPoint = temp;
	}
}

int main()
{
	ofstream out("data.txt");
	out.clear();
	Mat beforeImage = imread("binary.png", CV_8UC1); // 输入一张二值正方形图像
	Mat afterImage = Mat(beforeImage.rows, beforeImage.cols, CV_8UC1, Scalar(0));
	BorderDetect(beforeImage);
	out << "StartPoint : " << borderPointList[0] << endl;
	for (int i = 0; i < borderPointList.size(); ++i)
	{
		afterImage.at<uchar>(borderPointList[i].x, borderPointList[i].y) = 255;
		out << linkedCode[i];
		if((i + 1) % 50 == 0)
			out << endl;
	}
	out << endl;
	out.close();
	imshow("Before", beforeImage);
	imshow("After", afterImage);
	imwrite("After.png", afterImage);

	Mat lena = imread("lena.png", CV_8UC1);
	Mat GCMatrix1 = Mat(256, 256, CV_32FC1, Scalar(0));
	Mat GCMatrix2 = Mat(256, 256, CV_32FC1, Scalar(0));
	Mat GCMatrix3 = Mat(256, 256, CV_32FC1, Scalar(0));

	for (int i = 0; i < lena.rows; ++i)
		for (int j = 0; j < lena.cols - 1; ++j)
			GCMatrix1.at<float>(lena.at<uchar>(i, j), lena.at<uchar>(i, j + 1)) += 0.005f;

	for (int i = 0; i < lena.rows - 1; ++i)
		for (int j = 0; j < lena.cols; ++j)
			GCMatrix2.at<float>(lena.at<uchar>(i, j), lena.at<uchar>(i + 1, j)) += 0.005f;

	for (int i = 0; i < lena.rows - 1; ++i)
		for (int j = 0; j < lena.cols - 1; ++j)
			GCMatrix3.at<float>(lena.at<uchar>(i, j), lena.at<uchar>(i + 1, j + 1)) += 0.005f;

	imshow("GCMatrix1", GCMatrix1);
	imshow("GCMatrix2", GCMatrix2);
	imshow("GCMatrix3", GCMatrix3);

	normalize(GCMatrix1, GCMatrix1, 255, 0, CV_C);
	normalize(GCMatrix2, GCMatrix2, 255, 0, CV_C);
	normalize(GCMatrix3, GCMatrix3, 255, 0, CV_C);

	imwrite("水平方向共生矩阵.png", GCMatrix1);
	imwrite("垂直方向共生矩阵.png", GCMatrix2);
	imwrite("对角方向共生矩阵.png", GCMatrix3);

	waitKey();
}