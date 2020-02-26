#pragma once
#ifndef DETECTOR_H
#define DETECTOR_H

#include "opencv2/opencv.hpp"
#include "Debug.h"
#include "help_mat.h"

namespace chan {

	const std::string Stabilization = "test_data/PoliceFootage.mov";  // Name of your input file

#define resize_Factor 0.25 // Resize the input frames by this factor

#define descriptorDistanceThreshold 7.0//7.0; // Good matches are below "descriptorDistanceThreshold" times the minimum distance

#define correctionAmount 0.8 //	const double  //Set it between 0 and 1: The higher the smoother the footage will be

#define	akazeThreshold 4e-4 //	const float  //The higher the less features it will track

#define	SIFTThreshold 0.04

#define	SUFTThreshold 2000

#define	BRISKThreshold 30

#define ORB_edgeThreshold 31
#define ORB_fastThreshold 20

}

class detector{
	std::vector<cv::KeyPoint> kptsCurrent, kptsPrevious;
	cv::Mat descCurrent, descPrevious;

	///
	cv::Ptr<cv::FeatureDetector> AKAZA = cv::AKAZE::create(
		cv::AKAZE::DESCRIPTOR_MLDB, 0, 3, akazeThreshold, 4, 4,
		cv::KAZE::DIFF_PM_G2);
	///
	cv::Ptr<cv::xfeatures2d::SiftFeatureDetector> SIFT
		= cv::xfeatures2d::SiftFeatureDetector::create(
			0, 3, SIFTThreshold, 10, 1.6);
	///
	cv::Ptr<cv::FeatureDetector> BRISK = cv::BRISK::create(
		BRISKThreshold, 3, 1.0f);

	cv::Ptr<cv::FeatureDetector> ORB = cv::ORB::create(
		500, 1.2f, 8, ORB_edgeThreshold, 0, 2, ORB::HARRIS_SCORE, 31, ORB_fastThreshold);
	/*enum ScoreType {
		HARRIS_SCORE = 0,
		FAST_SCORE = 1
	};*/

	cv::Ptr<cv::xfeatures2d::SurfFeatureDetector> SURF
		= cv::xfeatures2d::SurfFeatureDetector::create(
			SUFTThreshold, 4, 3, false, false);
	

	// Descriptor brute force matching using hamming distance
	cv::BFMatcher matcher;

	std::vector<std::vector<cv::DMatch> > nn_matches;

	std::vector<cv::DMatch> matches;

	// Kalman filter to smooth the homographies over time
	cv::KalmanFilter kf;

	cv::Mat frameCorrected;
	cv::Mat previousH;

	bool kfInitialized = false;

	string str;
public:
	detector(cv::Mat& frame, string str) {
		this->str = str;
		this->matcher = cv::BFMatcher(cv::NORM_HAMMING);
		this->kf = cv::KalmanFilter(8, 8, 0, CV_64F);
		this->previousH = cv::Mat(3, 3, CV_64F);

		cv::resize(frame, frameCorrected,
			cv::Size(frame.cols * resize_Factor, frame.rows * resize_Factor));

		//mask : non
		if (str == "AKAZA") {
			// AKAZE features:
			// Fast Explicit Diffusion for Accelerated Features in Nonlinear Scale Spaces.
			// Pablo F. Alcantarilla, Jesús Nuevo and Adrien Bartoli.
			// In British Machine Vision Conference (BMVC), Bristol, UK, September 2013.

			// The initial homography is the identity transformation
			this->AKAZA->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
		}
		else if (str == "SIFT") {
			this->SIFT->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
		}
		else if (str == "BRISK") {
			this->BRISK->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
		}
		else if (str == "ORB") {
			this->ORB->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
		}
		else if (str == "SURF") {
			this->SURF->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
		}
		else {
			cout << "\n ** 올바른 detector를 설정하지 않아 AKEZA로 설정합니다. \n";
			str = "AKAZA";
			this->AKAZA->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
		}
		check_8UC1(descCurrent);

		cv::setIdentity(kf.measurementMatrix);
		cv::setIdentity(previousH);
	}//End detector

	bool Stabilization(cv::Mat& frame, cv::Mat& frame_clone, cv::Mat& frame_show, bool debug = 1);
};//End detector


bool detector::Stabilization(cv::Mat& frame, cv::Mat& frame_clone, cv::Mat& frame_show, bool debug) {

	//이전 값 저장
	kptsPrevious = kptsCurrent;
	descPrevious = descCurrent.clone();
	
	cv::resize(frame, frame_clone,
		cv::Size(frame.cols * resize_Factor, frame.rows * resize_Factor));
	frameCorrected = frame_clone.clone();

	kptsCurrent.clear();
	descCurrent = cv::Mat();

	std::vector<cv::Point2f> ptsCurrent;
	std::vector<cv::Point2f> ptsPrevious;

	if (str == "AKAZA") {
		this->AKAZA->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
	}
	else if (str == "SIFT") {
		this->SIFT->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
	}
	else if (str == "BRISK") {
		this->BRISK->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
	}
	else if (str == "ORB") {
		this->ORB->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
	}
	else if (str == "SURF") {
		this->SURF->detectAndCompute(frameCorrected, cv::noArray(), kptsCurrent, descCurrent);
	}
	else {
		//SURF
		//KAZE
		cout << "\n ** detector.h error : detector 설정에 문제가 있습니다. \n";
	}
	check_8UC1(descCurrent);
	
	// We havent retrieved enough keypoints
	if (kptsCurrent.size() < 20) {
		std::cout << "\n ** Stabilization.h error : havent retrieved enough keypoints \n";
		return 0;
	}
	//debug::show_wait(descCurrent, descPrevious);
	matcher.match(descCurrent, descPrevious, matches);
	//matcher.knnMatch(descCurrent, descPrevious, nn_matches, 2);
	
	double minDistance = 100;
	for (unsigned int i = 0; i < matches.size(); i++) {
		if (matches[i].distance < minDistance) {
			minDistance = matches[i].distance;
		}
	}

	for (unsigned int i = 0; i < matches.size(); i++) {
		if (matches[i].distance <
			std::max(25.0, descriptorDistanceThreshold * minDistance
			)) {
			ptsCurrent.push_back(kptsCurrent[matches[i].queryIdx].pt);
			ptsPrevious.push_back(kptsPrevious[matches[i].trainIdx].pt);
		}
	}


	// 호모 그래피를 계산하려면 각 이미지에 4 포인트 이상 필요
	// RANSAC로 인해 임계 값을 더 높게 설정
	if (ptsCurrent.size() < 10 || ptsPrevious.size() < 10) {
		std::cout << "\n ** Stabilization.h error : havent enough pts < 3 \n";
		return 0;
	}
	cv::Mat H = cv::findHomography(ptsPrevious, ptsCurrent, cv::RANSAC, ;
	//0 - 모든 점을 사용하는 정규 방법, 즉 최소 제곱 법(default)
	//RANSAC - RANSAC 기반의 강력한 방법
	//LMEDS - 최소 중간 값의 강력한 방법
	//RHO - PROSAC 기반의 강력한 방법


	// 칼만 필터 상태 초기화
	// 호모 그래피를 처음 계산할 때만 수행
	if (!kfInitialized) {
		kf.statePre = H.reshape(1, 9).rowRange(0, 8).clone();
		kf.statePost = H.reshape(1, 9).rowRange(0, 8).clone();
		kfInitialized = true;
	}
	
	// The homography calculation has failed
	if (H.cols < 3) {
		std::cout << "\n ** Stabilization.h error : H.cols < 3 \n";
		return 0;
	}

	// 칼만 필터 루프 : 예측 및 수정
	cv::Mat predicted = kf.predict();
	cv::Mat HEstimated = kf.correct(H.reshape(1, 9).rowRange(0, 8));
	HEstimated.push_back(cv::Mat(1, 1, CV_64F, cv::Scalar(1)));

	HEstimated = HEstimated.reshape(1, 3);
	
	// smoothing 양을 제어합니다.
	// 이전 weight가 많을수록 footage의 안정성이 향상됩니다
	cv::addWeighted(previousH, correctionAmount, cv::Mat::eye(3, 3, CV_64F),
		1 - correctionAmount, 0, previousH);

	cv::Mat HEstimated_ptsPrevious; // H_smoothed * previous_points
	cv::Mat HPrevious_ptsPrevious; //  H_used_to correct_previous_frame * previous_points

	// current와 previous사이의 호모그래피 :
	// 평활 한 호모그래피로 변형 된 previous point
	// 이전 보정 호모그래피로 변환 된 previous point
	// 현재 프레임을 수정하는 호모 그래피
	cv::perspectiveTransform(ptsPrevious, HEstimated_ptsPrevious,
		HEstimated);
	cv::perspectiveTransform(ptsPrevious, HPrevious_ptsPrevious, previousH);
	
	cv::Mat HCorrection = cv::findHomography(HEstimated_ptsPrevious,
		HPrevious_ptsPrevious, cv::RANSAC);
	
	cv::warpPerspective(frame_clone, frameCorrected, HCorrection,
		cv::Size(frame_clone.cols, frame_clone.rows), cv::INTER_CUBIC);
	
	previousH = HCorrection.clone();

	if (debug) {
		cv::drawKeypoints(frame_clone, kptsCurrent, frame_show);
		cv::hconcat(frame_show, frameCorrected, frame_show);
		cam::center_line(frame_show);
		putText(frame_show, str,
			Point(frame_show.cols/2, frame_show.rows - 10),
			FONT_HERSHEY_PLAIN,
			1,						//fontScale
			cv::Scalar(255, 255, 0),	//color
			1.5);							//thickness
	}
	//FONT_HERSHEY_SIMPLEX = 0,			//!< normal size sans-serif font
	//FONT_HERSHEY_PLAIN = 1,			//!< small size sans-serif font
	//FONT_HERSHEY_DUPLEX = 2,			//!< normal size sans-serif font (more complex than FONT_HERSHEY_SIMPLEX)
	//FONT_HERSHEY_COMPLEX = 3,			//!< normal size serif font
	//FONT_HERSHEY_TRIPLEX = 4,			//!< normal size serif font (more complex than FONT_HERSHEY_COMPLEX)
	//FONT_HERSHEY_COMPLEX_SMALL = 5,	//!< smaller version of FONT_HERSHEY_COMPLEX
	//FONT_HERSHEY_SCRIPT_SIMPLEX = 6,	//!< hand-writing style font
	//FONT_HERSHEY_SCRIPT_COMPLEX = 7,	//!< more complex variant of FONT_HERSHEY_SCRIPT_SIMPLEX
	//FONT_ITALIC = 16					//!< flag for italic font

	cv::resize(frameCorrected, frame_clone,
		cv::Size(frameCorrected.cols / resize_Factor * 0.7,
		frameCorrected.rows / resize_Factor * 0.7));
	cam::center_line(frame_clone);
	
	return 1;
}

#endif /// !EMOTION_H