#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;

float addPerPoint = 0.01f;
int houghNumber = 60; // 阈值 每个点值为0.01

class HoughPoint
{
public:
	float angle; // 角度
	float radium; // 半径
};

int main()
{
	Mat beforeImage = imread("binary.png", CV_8UC1); // 输入一张二值正方形图像
	Mat afterImage;
	Mat houghImage = Mat((int)beforeImage.rows*1.5f, 314, CV_32FC1);
	Mat houghImageAfterFilter = Mat((int)beforeImage.rows*1.5f, 314, CV_32FC1);

	beforeImage.copyTo(afterImage);

	for (int i = 0; i < beforeImage.rows; i++) {
		for (int j = 0; j < beforeImage.cols; j++) {
			if (beforeImage.at<uchar>(i, j) == 255) {
				for (int k = 0; k < houghImage.cols; k++) {
					int y = (i - beforeImage.rows / 2) * sin(k / 100.0f) + (j - beforeImage.cols / 2) * cos(k / 100.0f) + houghImage.rows / 2;
					houghImage.at<float>(y, k) += addPerPoint;
				}
			}
		}
	}

	for (int i = 0; i < houghImageAfterFilter.rows; i++) {
		for (int j = 0; j < houghImageAfterFilter.cols; j++) {
			if (houghImage.at<float>(i, j) > houghNumber * addPerPoint) {
				houghImageAfterFilter.at<float>(i, j) = 1.0f;
			}
			else {
				houghImageAfterFilter.at<float>(i, j) = 0.0f;
			}
		}
	}

	vector<HoughPoint> vec;
	for (int i = 0; i < houghImageAfterFilter.rows; i++) {
		for (int j = 0; j < houghImageAfterFilter.cols; j++) {
			if (houghImageAfterFilter.at<float>(i, j) == 1.0f) {
				HoughPoint temp;
				temp.angle = j / 100.0f;
				temp.radium = i - houghImageAfterFilter.rows / 2.0f;
				vec.push_back(temp);
			}
		}
	}

	for (int i = 0; i < vec.size(); i++) {
		cout << vec[i].angle << " " << vec[i].radium << endl;
		for (int j = 0; j < afterImage.cols; j++) {
			int y = vec[i].radium - (j - beforeImage.cols / 2) * cos(vec[i].angle) / sin(vec[i].angle) + afterImage.rows / 2;
			if (y < afterImage.rows && y >= 0)
				afterImage.at<uchar>(y, j) = 128;
		}
	}

	imshow("Before", beforeImage);
	imshow("HoughImage", houghImage);
	imshow("HoughImageAfterFilter", houghImageAfterFilter);
	imshow("After", afterImage);
	imwrite("After.png", afterImage);
	normalize(houghImage, houghImage, 255, 0, CV_C);
	normalize(houghImageAfterFilter, houghImageAfterFilter, 255, 0, CV_C);
	imwrite("HoughImage.png", houghImage);
	imwrite("HoughImageAfterFilter.png", houghImageAfterFilter);

	waitKey();
}