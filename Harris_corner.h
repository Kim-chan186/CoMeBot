#pragma once
#ifndef HARRIS_H
#define HARRIS_H
//해리스 코너 검출(Harris Corner Detection)
//검출된 코너를 이용해 다른 사진에서 유사한것(회전해있거나 크기가 바뀐 같은 Object)을 찾을 수 있다.
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>


using namespace cv;
using namespace std;

Mat src, src_gray;
int thresh = 200;
int max_thresh = 255;

const char* source_window = "Source image";
const char* corners_window = "Harris Corners detected";

void cornerHarris_demo(int, void*);

void harris_corner(int argc, char** argv)
{

	CommandLineParser parser(argc, argv, "{@input | building.jpg | input image}");
	src = imread(samples::findFile(parser.get<String>("@input")));

	if (src.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		cout << "Usage: " << argv[0] << " <Input image>" << endl;
		return;
	}

	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	namedWindow(source_window);
	createTrackbar("Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo);
	imshow(source_window, src);
	cornerHarris_demo(0, 0);
	return;

}
void cornerHarris_demo(int, void*)
{
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	Mat dst = Mat::zeros(src.size(), CV_32FC1);
	cornerHarris(src_gray, dst, blockSize, apertureSize, k);

	Mat dst_norm, dst_norm_scaled;
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	// 정규화
	// 가장 어두운 부분을 0으로, 가장 밝은 부분을 255로
	// 그 외의 부분은 0과 255의 상대적인 위치에 해당하는 값으로 정규화

	convertScaleAbs(dst_norm, dst_norm_scaled);//정밀도 부동 소수점 > 반 정밀도 부동소수점

	for (int i = 0; i < dst_norm.rows; i++)
	{
		for (int j = 0; j < dst_norm.cols; j++)
		{
			if ((int)dst_norm.at<float>(i, j) > thresh)
			{
				circle(dst_norm_scaled, Point(j, i), 5, Scalar(0), 2, 8, 0);
			}//중간에 계속 연산해서 오류(가 나는 것처럼 보이는 연산량 증가)를 야기
		}
	}
	namedWindow(corners_window);
	imshow(corners_window, dst_norm_scaled);
}

#endif /// !EMOTION_H