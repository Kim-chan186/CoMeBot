<<<<<<< HEAD
//Gui Çì´õÆÄÀÏ Gui¸¦ À§ÇÑ ÇÔ¼öµé
=======
//Gui í—¤ë”íŒŒì¼ Guië¥¼ ìœ„í•œ í•¨ìˆ˜ë“¤
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179

//main¹®¿¡¼­ »ç¿ëÇÒ ¶§ gui_main(Point emotion, int pleasantness, int energy);ÇÔ¼ö¸¦ ³Ö¾îÁÖ¼¼¿ä

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>


using namespace cv;
using namespace std;

namespace Gui {
	const double PI = 3.14;
	Scalar black(0, 0, 0);
	Scalar red(0, 0, 255);
	Scalar green(0, 180, 0);
	Scalar blue(255, 0, 0);
	Scalar white(255, 255, 255);
	Scalar yellow(0, 255, 255);
	Scalar cyan(255, 255, 0);
	Scalar magenta(255, 0, 255);
	Scalar gray(150, 150, 150);
	Scalar purple(255, 0, 127);

	typedef struct {
		double r;       // a fraction between 0 and 1
		double g;       // a fraction between 0 and 1
		double b;       // a fraction between 0 and 1
	} rgb;

	typedef struct {
		double h;       // angle in degrees
		double s;       // a fraction between 0 and 1
		double v;       // a fraction between 0 and 1
	} hsv;

	static rgb   hsv2rgb(hsv in);
	static hsv  rgb2hsv(rgb in);

	Mat readimg(String name, int x, int y);
	Point percent(Point emotion);
	Scalar deg2hue(int x, int y);
	rgb hsv2rgb(hsv in);
	void color_line_chart(Mat& img, Point emotion);
	void stick_chart(Mat& img, int pleasantness, int energy);
	Scalar trans_color(int percent, Scalar color);
}

Point circle_emotionn;
int stick_pleasantness;
int stick_energy;

<<<<<<< HEAD
Mat color_img = Gui::readimg("img/whale.jpg", 384, 384);	//¿øÇü»ö»ó±×·¡ÇÁ ÀÌ¹ÌÁö ºÒ·¯¿À±â
Mat stick_img = Gui::readimg("img/stick.jpg", 190, 220);	//¸·´ë±×·¡ÇÁ ÀÌ¹ÌÁö ºÒ·¯¿À±â

void gui_main(Point circle_emotion, int stick_pleasantness, int stick_energy)
{
	Gui::color_line_chart(color_img, circle_emotion);	//ÁÂÇ¥°ª¿¡ µû¶ó È­»ìÇ¥±×·ÁÁÖ´Â ÇÔ¼ö
	Gui::stick_chart(stick_img, stick_pleasantness, stick_energy);		//ÁÂÇ¥°ªÀ» ¸·´ë±×·¡ÇÁ·Î ³ªÅ¸³»ÁÖ´Â ÇÔ¼ö
=======
Mat color_img = Gui::readimg("img/whale.jpg", 384, 384);	//ì›í˜•ìƒ‰ìƒê·¸ëž˜í”„ ì´ë¯¸ì§€ ë¶ˆëŸ¬ì˜¤ê¸°
Mat stick_img = Gui::readimg("img/stick.jpg", 190, 220);	//ë§‰ëŒ€ê·¸ëž˜í”„ ì´ë¯¸ì§€ ë¶ˆëŸ¬ì˜¤ê¸°

void gui_main(Point circle_emotion, int stick_pleasantness, int stick_energy)
{
	Gui::color_line_chart(color_img, circle_emotion);	//ì¢Œí‘œê°’ì— ë”°ë¼ í™”ì‚´í‘œê·¸ë ¤ì£¼ëŠ” í•¨ìˆ˜
	Gui::stick_chart(stick_img, stick_pleasantness, stick_energy);		//ì¢Œí‘œê°’ì„ ë§‰ëŒ€ê·¸ëž˜í”„ë¡œ ë‚˜íƒ€ë‚´ì£¼ëŠ” í•¨ìˆ˜
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
}

//ÀÌ¹ÌÁöÆÄÀÏ ºÒ·¯¿À°í »çÀÌÁî Á¶ÀýÇÏ´Â ÇÔ¼ö
Mat Gui::readimg(String name, int x, int y)  //(ÆÄÀÏ¸í, xÃàsize, yÃàsize)
{
	Mat img;

	img = imread(name, IMREAD_COLOR);
	resize(img, img, Size(x, y));

	return img;
}

//°¨Á¤°ª whaleÀÌ¹ÌÁö¿¡ ¸Â°Ô ºñÀ² ¸ÂÃß´Â ÇÔ¼ö
Point Gui::percent(Point emotion)  //(°¨Á¤°ª)
{
	if (emotion.x >= 0)
	{
		emotion.x = (int)(emotion.x *(155.0 / 100.0));
	}
	else if (emotion.x < 0)
	{
		emotion.x = (int)(emotion.x *(152.0 / 100.0));
	}

	if (emotion.y >= 0)
	{
		emotion.y = (int)(emotion.y *(153.0 / 100.0));
	}
	else if (emotion.y < 0)
	{
		emotion.y = (int)(emotion.y *(152.0 / 100.0));
	}

	return emotion;
}

//ÁÂÇ¥°ª ¹Þ¾Æ¼­ °¢µµ¿¡µû¸¥ »öÀ¸·Î º¯È¯ÇØÁÖ´Â ÇÔ¼ö
Scalar Gui::deg2hue(int x, int y) //(°¨Á¤ xÁÂÇ¥, °¨Á¤ yÁÂÇ¥)
{
	//ÁÂÇ¥°¢µµ->hue°ªÀ¸·Î º¯È¯
	float degree;
	int hue;

	degree = atan2(x, y) * 180 / PI;

	//°¢µµ°ªÀÌ -180~180 ¹üÀ§·Î Ç¥ÇöµÇ±â ¶§¹®¿¡ 0~360µµ·Î º¯È¯ÇÏ±âÀ§ÇÑ Á¶°Ç¹®
	if (degree >= 0)
	{
		hue = (int)degree / 2;
	}
	else if (degree < 0)
	{
		degree += 360.0;
		//printf("%.f\n", degree);
		hue = (int)degree / 2;
	}

	//hue°ª color·Î º¯È¯
	rgb R;
	hsv P;
	int r, g, b;
	P.h = hue * 2;
	P.s = 1;
	P.v = 1;
	R = Gui::hsv2rgb(P);
	r = (int)(R.r * 255);
	g = (int)(R.g * 255);
	b = (int)(R.b * 255);

	return Scalar(b, g, r);
}

//hsv->rgb·Î ¹Ù²Ù´Â ÇÔ¼ö
Gui::rgb Gui::hsv2rgb(hsv in)
{
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;

	if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	case 5:
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}

<<<<<<< HEAD
//rgb->hsv·Î ¹Ù²Ù´Â ÇÔ¼ö
=======
//rgb->hsvë¡œ ë°”ê¾¸ëŠ” í•¨ìˆ˜
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
Gui::hsv Gui::rgb2hsv(rgb in)
{
	hsv        out;
	double      min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min  < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max  > in.b ? max : in.b;

	out.v = max;                                // v
	delta = max - min;
	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);                  // s
	}
	else {
		// if max is 0, then r = g = b = 0
		// s = 0, v is undefined
		out.s = 0.0;
		out.h = 0.0;                            // its now undefined
		return out;
	}
	if (in.r >= max)                           // > is bogus, just keeps compilor happy
		if (delta == 0) {
			out.h = 0.0;
		}
		else {
			out.h = (in.g - in.b) / delta;        // between yellow & magenta
		}
	else
		if (in.g >= max)
			out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
		else
			out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

	out.h *= 60.0;                              // degrees

	if (out.h < 0.0)
		out.h += 360.0;

	return out;
}

<<<<<<< HEAD
//ÁÂÇ¥°ª¿¡ µû¶ó È­»ìÇ¥±×·ÁÁÖ´Â ÇÔ¼ö
void Gui::color_line_chart(Mat& img, Point emotion)  //(whale¿øÇü±×·¡ÇÁÀÌ¹ÌÁö, °¨Á¤°ª)
=======
//ì¢Œí‘œê°’ì— ë”°ë¼ í™”ì‚´í‘œê·¸ë ¤ì£¼ëŠ” í•¨ìˆ˜
void Gui::color_line_chart(Mat& img, Point emotion)  //(whaleì›í˜•ê·¸ëž˜í”„ì´ë¯¸ì§€, ê°ì •ê°’)
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
{
	int hue;
	Scalar color;
	Point center, result;
	int max_x = 348;
	int max_y = 345;
	int min_x = 39;
	int min_y = 38;
	
	double R;
	double theta;
	Mat img_circle = img.clone();
	emotion = Gui::percent(emotion);

	center.x = img.rows / 2;			//ÀÌ¹ÌÁö Áß½ÉÁÂÇ¥ÀúÀå
	center.y = img.cols / 2;

	//°¨Á¤ÁÂÇ¥¸¦ ¿øÇü±×·¡ÇÁ ¹üÀ§ ³»ÀÇ °ªÀ¸·Î º¯È¯ÇÏ´Â°ÍÀº ¾ÆÁ÷ ¼öÁ¤ÀÌ ÇÊ¿äÇÔ
	result.x = center.x + emotion.x;				//°¨Á¤°ªÀÇ ¿øÁ¡À» ÀÌ¹ÌÁö Áß½ÉÀ¸·Î ¿Å±è
	result.y = img.cols - (center.y + emotion.y);	//yÁÂÇ¥ÀÇ 0Á¡ÀÌ È­¸é»ó´Ü¿¡¼­ ½ÃÀÛÇÏ¹Ç·Î ¹ÝÀü½ÃÅ´

	color = deg2hue(emotion.x, emotion.y);			//ÁÂÇ¥¿¡ µû¸¥ bgr°ª ¹Þ¾Æ¿È

<<<<<<< HEAD
	circle(img_circle, Point(center.x, center.y), 4, gray, -1);	//±×·¡ÇÁ Áß½É
	line(img_circle, Point(min_x, center.y), Point(max_x, center.y), gray, 1);	//xÃà
	line(img_circle, Point(center.x, min_y), Point(center.x, max_y), gray, 1);	//yÃà
	arrowedLine(img_circle, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//È­»ìÇ¥±×¸®±â
=======
	circle(img_circle, Point(center.x, center.y), 4, gray, -1);	//ê·¸ëž˜í”„ ì¤‘ì‹¬
	line(img_circle, Point(min_x, center.y), Point(max_x, center.y), gray, 1);	//xì¶•
	line(img_circle, Point(center.x, min_y), Point(center.x, max_y), gray, 1);	//yì¶•
	arrowedLine(img_circle, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//í™”ì‚´í‘œê·¸ë¦¬ê¸°
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179

	imshow("grdual_emotion", img_circle);
}

<<<<<<< HEAD
//ÁÂÇ¥°ªÀ» ¸·´ë±×·¡ÇÁ·Î ³ªÅ¸³»ÁÖ´Â ÇÔ¼ö
void Gui::stick_chart(Mat& img, int pleasantness, int energy) //(¸·´ë±×·¡ÇÁÀÌ¹ÌÁö, °¨Á¤°ª)
=======
//ì¢Œí‘œê°’ì„ ë§‰ëŒ€ê·¸ëž˜í”„ë¡œ ë‚˜íƒ€ë‚´ì£¼ëŠ” í•¨ìˆ˜
void Gui::stick_chart(Mat& img, int pleasantness, int energy) //(ë§‰ëŒ€ê·¸ëž˜í”„ì´ë¯¸ì§€, ê°ì •ê°’)
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
{
	int center_y = 100; //¸·´ë±×·¡ÇÁ Áß½ÉÃà
	Point result;
	Mat img_stick = img.clone();
<<<<<<< HEAD
	//°¨Á¤°ª(-100~100¹üÀ§¸¦ ¸·´ë±×·¡ÇÁ -90~90¹üÀ§·Î ³ªÅ¸³»´Â½Ä
=======
	//ê°ì •ê°’(-100~100ë²”ìœ„ë¥¼ ë§‰ëŒ€ê·¸ëž˜í”„ -90~90ë²”ìœ„ë¡œ ë‚˜íƒ€ë‚´ëŠ”ì‹
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
	result.x = (int)((pleasantness / 10) * 9);
	result.y = (int)((energy / 10) * 9);

	if (pleasantness >= 0) //°¨Á¤ÀÇ xÁÂÇ¥(±àÁ¤,ºÎÁ¤)°¡ ¾ç¼öÀÌ¸é ÃÊ·Ï»öÀ¸·Î ¸·´ë±×·¡ÇÁ Ã¤¿ò
	{
		green = Gui::trans_color(pleasantness, green);
		rectangle(img_stick, Point(31, center_y), Point(74, center_y - result.x), green, -1);
	}
	else                //°¨Á¤ÀÇ xÁÂÇ¥(±àÁ¤,ºÎÁ¤)°¡ À½¼öÀÌ¸é º¸¶ó»öÀ¸·Î ¸·´ë±×·¡ÇÁ Ã¤¿ò
	{
<<<<<<< HEAD
		purple = Gui::trans_color(pleasantness, purple);
		rectangle(img_stick, Point(31, center_y), Point(74, center_y - result.x), purple, -1);
	}
=======
		green = Gui::trans_color(pleasantness, green);
		rectangle(img_stick, Point(31, center_y), Point(74, center_y - result.x), green, -1);
	}
	else                //ê°ì •ì˜ xì¢Œí‘œ(ê¸ì •,ë¶€ì •)ê°€ ìŒìˆ˜ì´ë©´ ë³´ë¼ìƒ‰ìœ¼ë¡œ ë§‰ëŒ€ê·¸ëž˜í”„ ì±„ì›€
	{
		purple = Gui::trans_color(pleasantness, purple);
		rectangle(img_stick, Point(31, center_y), Point(74, center_y - result.x), purple, -1);
	}
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179

	if (energy >= 0) //°¨Á¤ÀÇ yÁÂÇ¥(¿¡³ÊÁö)°¡ ¾ç¼öÀÌ¸é »¡°£»öÀ¸·Î ¸·´ë±×·¡ÇÁ Ã¤¿ò
	{
		red = Gui::trans_color(energy, red);
		rectangle(img_stick, Point(116, center_y), Point(159, center_y - result.y), red, -1);
	}
<<<<<<< HEAD
	else                //°¨Á¤ÀÇ yÁÂÇ¥(¿¡³ÊÁö)°¡ À½¼öÀÌ¸é ÆÄ¶õ»öÀ¸·Î ¸·´ë±×·¡ÇÁ Ã¤¿ò
=======
	else                //ê°ì •ì˜ yì¢Œí‘œ(ì—ë„ˆì§€)ê°€ ìŒìˆ˜ì´ë©´ íŒŒëž€ìƒ‰ìœ¼ë¡œ ë§‰ëŒ€ê·¸ëž˜í”„ ì±„ì›€
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
	{
		blue = Gui::trans_color(energy, blue);
		rectangle(img_stick, Point(116, center_y), Point(159, center_y - result.y), blue, -1);
	}

	imshow("stick_emotion", img_stick);
}

Scalar Gui::trans_color(int emotion, Scalar color)
{
	if (emotion < 0)
	{
		emotion = emotion*(-1);
	}
	double percent = (double)emotion / 100.0;

<<<<<<< HEAD
	//°¨Á¤Á¤µµ¿¡ µû¶ó color Ã¤µµ º¯È­
=======
	//ê°ì •ì •ë„ì— ë”°ë¼ color ì±„ë„ ë³€í™”
>>>>>>> f9fc032c0772be8da8d79cb98b2de85aa773a179
	rgb R;
	hsv P;
	int r, g, b;
	double h, s, v;
	R.b = (double)color[0]/255.0;
	R.g = (double)color[1]/255.0;
	R.r = (double)color[2]/255.0;
	P = Gui::rgb2hsv(R);
	P.h = P.h;
	P.s = P.s*percent;
	P.v = P.v;
	R = Gui::hsv2rgb(P);
	r = (int)(R.r * 255);
	g = (int)(R.g * 255);
	b = (int)(R.b * 255);

	return Scalar(b, g, r);
}
