#pragma once
<<<<<<< HEAD:cpp/service.h
//∆Ì¿«∏¶ ¿ß«— ±‚¥…µÈ¿ª ¡§¿««— «Ï¥ı
=======
//Ìé∏ÏùòÎ•º ÏúÑÌïú Í∏∞Îä•Îì§ÏùÑ Ï†ïÏùòÌïú Ìó§Îçî
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h

#include <chrono>
#include <iostream>

namespace chan {
<<<<<<< HEAD:cpp/service.h
	//** error √‚∑¬
	//* sys √‚∑¬

	double dTime; // 1loop ¥Á ∞…∏Æ¥¬ Ω√∞£
=======
	//** error Ï∂úÎ†•
	//* sys Ï∂úÎ†•

	double dTime; // 1loop Îãπ Í±∏Î¶¨Îäî ÏãúÍ∞Ñ
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
	
	int CH_count = -1;
	std::chrono::system_clock::time_point tpStart, tpEnd;
	
	/*
	
<<<<<<< HEAD:cpp/service.h
	# Ω√¿€√¯¡§
	forπÆ¿Ã≥™ whileπÆ æ»ø° ≥÷æÓº≠ ¿⁄µø¿∏∑Œ Ω√∞£¡÷±‚∏¶ √¯¡§«ÿ¡÷¥¬ «‘ºˆ

	- Check_Time(int Loop = 1000)

	- for (int i = 0; (i < 1000)&chan::Check_Time(); i++){}
	- while(chan::Check_Time()){}

	- √— Ω√∞£¿Ã æ∆¥œ∂Û ¡¶æÓ¡÷±‚ ∏∏≈≠∏∏(1∑Á«¡)

	∏Óπ¯ ∑Á«¡∏¶ ºˆ«‡«—µ⁄ Ω√∞£¿ª √¯¡§«“ ∞Õ¿Œ¡ˆ∏¶ ¿‘∑¬¿∏∑Œ πﬁ¿Ω
	-√ﬂ»ƒ ¿©µµøÏ ªÛø° ±€¿⁄∑Œ ∂ÁøÏ¥¬ ∞Õ¿∏∑Œ ºˆ¡§ øπ¡§
	-√ ±‚»≠ «‘ºˆ∏¶ ∏∏µÈ øπ¡§
=======
	# ÏãúÏûëÏ∏°Ï†ï
	forÎ¨∏Ïù¥ÎÇò whileÎ¨∏ ÏïàÏóê ÎÑ£Ïñ¥ÏÑú ÏûêÎèôÏúºÎ°ú ÏãúÍ∞ÑÏ£ºÍ∏∞Î•º Ï∏°Ï†ïÌï¥Ï£ºÎäî Ìï®Ïàò
	- Check_Time(int Loop = 1000)
	- for (int i = 0; (i < 1000)&chan::Check_Time(); i++){}
	- while(chan::Check_Time()){}
	- Ï¥ù ÏãúÍ∞ÑÏù¥ ÏïÑÎãàÎùº Ï†úÏñ¥Ï£ºÍ∏∞ ÎßåÌÅºÎßå(1Î£®ÌîÑ)
	Î™áÎ≤à Î£®ÌîÑÎ•º ÏàòÌñâÌïúÎí§ ÏãúÍ∞ÑÏùÑ Ï∏°Ï†ïÌï† Í≤ÉÏù∏ÏßÄÎ•º ÏûÖÎ†•ÏúºÎ°ú Î∞õÏùå
	-Ï∂îÌõÑ ÏúàÎèÑÏö∞ ÏÉÅÏóê Í∏ÄÏûêÎ°ú ÎùÑÏö∞Îäî Í≤ÉÏúºÎ°ú ÏàòÏ†ï ÏòàÏ†ï
	-Ï¥àÍ∏∞Ìôî Ìï®ÏàòÎ•º ÎßåÎì§ ÏòàÏ†ï
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
	*/

	bool Check_Time(int Loop = 1000) {
		CH_count++;

		if(CH_count == 0) {
<<<<<<< HEAD:cpp/service.h
			tpStart = std::chrono::system_clock::now(); //Ω√∞£ √¯¡§ Ω√¿€ 
			printf("Ω√¿€");
=======
			tpStart = std::chrono::system_clock::now(); //ÏãúÍ∞Ñ Ï∏°Ï†ï ÏãúÏûë 
			printf("ÏãúÏûë");
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
			return 1;
		}
		else if (CH_count < Loop){
			return 1;
		}
		else if(CH_count == Loop) {
<<<<<<< HEAD:cpp/service.h
			tpEnd = std::chrono::system_clock::now(); //Ω√∞£ √¯¡§ ≥°
			dTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() /1e6 / Loop;
			printf("Loop Time: %.3lf ms\n", dTime);

			tpStart = std::chrono::system_clock::now(); //Ω√∞£ √¯¡§ Ω√¿€
=======
			tpEnd = std::chrono::system_clock::now(); //ÏãúÍ∞Ñ Ï∏°Ï†ï ÎÅù
			dTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tpEnd - tpStart).count() /1e6 / Loop;
			printf("Loop Time: %.3lf ms\n", dTime);

			tpStart = std::chrono::system_clock::now(); //ÏãúÍ∞Ñ Ï∏°Ï†ï ÏãúÏûë
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
			CH_count = 0;
			return 1;
		}
		else {
			printf("\n ** error Check_Time!! \n");
			return 1;
		}
		/*//TEST Check_Time
		for (int i = 0; (i < 1000)&chan::Check_Time(); i++) {
<<<<<<< HEAD:cpp/service.h
			cout << "!";//¥ √ﬂ±‚ ¿ß«ÿ µπˆ±Î ∏µÂ∑Œ ∫ÙµÂ
			for (int j = 0; j < 1000; j++) {
				for (int k = 0; k < 1000; k++) {
					//Ω√∞£º“∏
=======
			cout << "!";//Îä¶Ï∂îÍ∏∞ ÏúÑÌï¥ ÎîîÎ≤ÑÍπÖ Î™®ÎìúÎ°ú ÎπåÎìú
			for (int j = 0; j < 1000; j++) {
				for (int k = 0; k < 1000; k++) {
					//ÏãúÍ∞ÑÏÜåÎ™®
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
					int a = 100;
					int b = 100;
					int c = a = a * b;
					c = b = a * c - b;
<<<<<<< HEAD:cpp/service.h
					//Ω√∞£º“∏_end
=======
					//ÏãúÍ∞ÑÏÜåÎ™®_end
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
		}}}*/

	}//end Check_Time
	

	int* int_track_vaule = nullptr;

	double* double_track_vaule = nullptr;

<<<<<<< HEAD:cpp/service.h
	//∫¸∏• ∆Æ∑∫πŸ
	//¿Ã»ƒ ≈¨∑°Ω∫∑Œ ∫Ø∞Ê
=======
	//Îπ†Î•∏ Ìä∏Î†âÎ∞î
	//Ïù¥ÌõÑ ÌÅ¥ÎûòÏä§Î°ú Î≥ÄÍ≤Ω
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
	static void on_trackbar(int _value , void* _userdata) {
		_userdata = (double*)(int*)(int)((*(int*)_value) / 10.0);
	}


	void fast_trackbar(double* _value) {
		cv::namedWindow("track Bar");
		double_track_vaule = _value;
		cv::createTrackbar("value / 10.0", "track Bar", int_track_vaule, 1000, on_trackbar, _value);
		//setTrackbarPos("low threshold", "Canny", 50);
		//00 = getTrackbarPos('low threshold', 'Canny')
	}


}
<<<<<<< HEAD:cpp/service.h


=======
>>>>>>> 74479fc5bfa5f5b21941707898be987380fc147f:service.h
