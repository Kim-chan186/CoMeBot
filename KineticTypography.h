

#pragma once
#ifndef KINETIC_TYPOGRAGHY_H
#define KINETIC_TYPOGRAGHY_H

#include <opencv2/core.hpp>
#include <gui_test.h>

#include "Debug.h"


namespace chan {

	const double typo_percent = 0.1; //10%

	int tp_fps = 33; //10%

	class typo {

		Gui::EWords text;

		float hue;				//0~180

		float transparency;		//0 : 투명, 100 : 불투명 			출력 : int형

		cv::Point point;		// -100 ~ 100

		float font_scale;			//0~50,								출력 : int형


		cv::Vec2d vec;

		int _delta = 5;			//아직 안씀

		int* _time_seed = &tp_fps;


	public:

		void delta() {

			point = point + (cv::Point)vec;


			if (!*_time_seed) {
				cout << "\n ** K_Typo.h error time_seed is zero !!\n";
				return;
			}

			double num = pow(1 - typo_percent, 1 / (1000.0 / (*_time_seed) - 1));
			cv::Vec2d delta = vec * num;
			this->vec = delta;
		}



		//(text, hue, transparency(0~100), point(+-100), font_scale(0~50));
		void show() {
			Gui::brain_typo(
				this->text,
				this->hue, 
				this->transparency,
				this->point,
				this->font_scale
			);
		}

		typo();

		
		typo(
			string str,
			float hue,
			float transparency,
			cv::Point point,
			float font_scale,
			cv::Vec2d vec,
			int* _time_seed
		);

		~typo() {};

	};//end typo 



	// ** 생성자 ** //

	typo::typo() {
		this->text = EW::놀라;
		this->hue = 0;
		this->transparency = 80;
		this->point = cv::Point(0, 0);
		this->font_scale = 7;
	}



	typo::typo(
		string str, 
		float hue, 
		float transparency, 
		cv::Point point, 
		float font_scale, 
		cv::Vec2d vec,
		int* _time_see)
	{
		this->text = EW::놀라;
		this->hue = hue;
		this->transparency = transparency;
		this->point = point;
		this->font_scale = font_scale;
		this->vec = vec;
		this->_time_seed = _time_see;
	}
}




#endif /// !EMOTION_H