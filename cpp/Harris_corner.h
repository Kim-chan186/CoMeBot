#pragma once
#ifndef HARRIS_H
#define HARRIS_H
//�ظ��� �ڳ� ����(Harris Corner Detection)
//����� �ڳʸ� �̿��� �ٸ� �������� �����Ѱ�(ȸ�����ְų� ũ�Ⱑ �ٲ� ���� Object)�� ã�� �� �ִ�.
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

	CommandLineParser parser(argc, argv, "{@input | test_data/building.jpg | input image}");
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
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, noArray());
	// ����ȭ
	// ���� ��ο� �κ��� 0����, ���� ���� �κ��� 255��
	// �� ���� �κ��� 0�� 255�� ������� ��ġ�� �ش��ϴ� ������ ����ȭ

	convertScaleAbs(dst_norm, dst_norm_scaled);//���е� �ε� �Ҽ��� > �� ���е� �ε��Ҽ���

	for (int i = 0; i < dst_norm.rows; i++)
	{
		for (int j = 0; j < dst_norm.cols; j++)
		{
			if ((int)dst_norm.at<float>(i, j) > thresh)
			{
				circle(dst_norm_scaled, Point(j, i), 5, Scalar(0), 2, 8, 0);
			}//�߰��� ��� �����ؼ� ����(�� ���� ��ó�� ���̴� ���귮 ����)�� �߱�
		}
	}
	namedWindow(corners_window);
	imshow(corners_window, dst_norm_scaled);
}

#endif /// !EMOTION_H