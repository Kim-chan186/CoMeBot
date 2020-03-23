#pragma once
#ifndef SIFT_H
#define SIFT_H
// Scale-Invariant Feature Transform

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>//Thanks to Alessandro

//https://github.com/opencv/opencv_contrib/releases 4.1.2
//extra modules contrib 추가 필요

void SIFT()
{
	const cv::Mat input = cv::imread("test_data/building.jpg", 0); //Load as grayscale

	if (input.empty()) {
		printf(" ** SIFT error : is empty\n");
	}
	else if (input.channels() != 1) {
		printf(" ** SIFT error : is not grayscale \n");
	}

	cv::Ptr<cv::xfeatures2d::SiftFeatureDetector> detector
		= cv::xfeatures2d::SiftFeatureDetector::create(0, 3, 0.15, 10, 1.6);

	std::vector<cv::KeyPoint> keypoints;
	detector->detect(input, keypoints);

	// Add results to image and save.
	cv::Mat output;
	cv::drawKeypoints(input, keypoints, output);
	imshow("SIFT", output);
	///cv::imwrite("sift_result.jpg", output);
}

#endif /// !EMOTION_H