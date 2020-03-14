//Gui 헤더파일 Gui를 위한 함수들

/*main문에서 사용할 때 
gui_main(Point gradually_emotion, Point immediately_emotion, int pleasantness, int energy);함수를 넣어주세요
brain_typo(EWords text, float hue, int transparency, Point point, int font_scale); Ewords text는 emotion_grid에 있는 단어명(ex:놀라,재미...)(참조※namespace EW),
																				   transparency(투명도)는 0(투명)~100(불투명)사이의 값,
																				   point는 단어위치좌표 원점(0,0) max는(100,100), min는(-100,-100)임
																				   font_scale은 글자폰트크기 (0~50)사이의 값 적정사이즈는 10~20정도 (0~5)정도는 쥐똥만하고, 30이상은 엄청큼 적절한 사이즈를 찾으시길!
																				   (참고※폰트굵기는 정수형으로밖에 지정이 안되서 크기에 따라 굵기가 바뀌게 해놓긴했지만 불연속적이여서 부자연스러움이 있음)
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

namespace Gui {
	//Gui관련 함수,변수
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

	struct rgb {
		double r;       // a fraction between 0 and 1
		double g;       // a fraction between 0 and 1
		double b;       // a fraction between 0 and 1
	};

	struct hsv {
		double h;       // angle in degrees
		double s;       // a fraction between 0 and 1
		double v;       // a fraction between 0 and 1
	};

	static rgb   hsv2rgb(hsv in);
	static hsv  rgb2hsv(rgb in);

	struct EWords
	{
		Point p;	//감정단어 좌표
		String text;	//감정단어 
	};

	struct brain_data
	{
		Mat img;
		EWords text;
		float hue;
		int transparency;
		Point point;
		int font_scale;
	};

	Mat readimg(String name, int x, int y);
	Point percent(Point emotion);
	float xy2deg(int x, int y);
	Scalar deg2hue(float degree);
	rgb hsv2rgb(hsv in);
	Mat color_line_chart(Mat img, Point emotion);
	Mat stick_chart(Mat& img, int pleasantness, int energy);
	Mat emotion_word_point(Mat& img, Point word);
	Mat draw_rect(Mat img, int p, int emotion, Scalar color);
	Scalar trans_color(int percent, Scalar color);
	Scalar trans_text_color(int emotion, Scalar color, int bl_n_wh);
	Scalar emotion_word_color(Point word);
	brain_data whale_brain(brain_data data);
	void combine_imshow(Mat circleL, Mat circleS, Mat stick);
}

namespace EW {
	//감정단어관련 Point
	//원본 이미지(emotion_grid)사이즈 (840,737)
	Point zeroP(424, 368); //그래프 원점
	//영어단어는 그냥 사전검색해서 match한거라 품사들이 일관적이지 않고 잘못되었을수있음 추후 수정
	Gui::EWords 놀라 = { { 506, 323 },"surprise" };
	Gui::EWords 흥분 = { { 521, 219 },"upset" };
	Gui::EWords 설레 = { { 604, 368 },"throb" };
	Gui::EWords 열망 = { { 626, 333 },"desire" };
	Gui::EWords 사랑 = { { 668, 330 },"love" };
	Gui::EWords 속시 = { { 648, 301 },"refresh" };
	Gui::EWords 후련 = { { 671, 297 },"relieved" };
	Gui::EWords 감탄 = { { 694, 302 },"amazed" };
	Gui::EWords 자랑 = { { 711, 289 },"proud" };
	Gui::EWords 긍지 = { { 694, 279 },"pride" };
	Gui::EWords 뿌듯 = { { 665, 274 },"brimful" };
	Gui::EWords 감격 = { { 676, 261 },"inspiring" };
	Gui::EWords 재미 = { { 695, 255 },"fun" };
	Gui::EWords 부럽 = { { 507, 419 },"envy" };
	Gui::EWords 그립 = { { 480, 537 },"miss" };
	Gui::EWords 연민 = { { 446, 662 },"compassion" };
	Gui::EWords 애틋 = { { 546, 546 },"fond" };
	Gui::EWords 찡하 = { { 549, 521 },"touch" };
	Gui::EWords 쑥쓰 = { { 408, 543 },"unbecoming" };
	Gui::EWords 부끄 = { { 390, 561 },"shy" };
	Gui::EWords 미안 = { { 391, 591 },"sorry" };
	Gui::EWords 불쌍 = { { 373, 629 },"poor" };
	Gui::EWords 안티 = { { 373, 643 },"anti" };
	Gui::EWords 측은 = { { 403, 676 },"pity" };
	Gui::EWords 민망 = { { 358, 560 },"awkward" };
	Gui::EWords 당혹 = { { 345, 483 },"perplexed" };
	Gui::EWords 창피 = { { 340, 521 },"embarrassed" };
	Gui::EWords 후회 = { { 321, 513 },"regret" };
	Gui::EWords 죄책 = { { 319, 533 },"guilty" };
	Gui::EWords 허전 = { { 303, 633 },"empty" };
	Gui::EWords 씁쓸 = { { 286, 614 },"bitter" };
	Gui::EWords 초조 = { { 284, 533 },"nervous" };
	Gui::EWords 불안 = { { 284, 515 },"anxiety" };
	Gui::EWords 절박 = { { 280, 484 },"desperate" };
	Gui::EWords 착잡 = { { 279, 558 },"intricacy" };
	Gui::EWords 허무 = { { 276, 594 },"notingness" };
	Gui::EWords 지루 = { { 259, 489 },"boring" };
	Gui::EWords 상실 = { { 257, 567 },"lose" };
	Gui::EWords 슬픔 = { { 259, 583 },"sadness" };
	Gui::EWords 서글 = { { 252, 624 },"sorrowful" };
	Gui::EWords 우울 = { { 236, 587 },"depression" };
	Gui::EWords 소외 = { { 239, 557 },"isolation" };
	Gui::EWords 심란 = { { 240, 528 },"disturbed" };
	Gui::EWords 속상 = { { 242, 481 },"hurt" };
	Gui::EWords 야속 = { { 240, 378 },"unkind" };
	Gui::EWords 참담 = { { 234, 460 },"dismal" };
	Gui::EWords 괴롭 = { { 214, 399 },"bully" };
	Gui::EWords 비참 = { { 210, 443 },"misery" };
	Gui::EWords 좌절 = { { 204, 481 },"frustration" };
	Gui::EWords 불행 = { { 192, 428 },"unfortunate" };
	Gui::EWords 절망 = { { 196, 409 },"despair" };
	Gui::EWords 황당 = { { 314, 284 },"absurd" };
	Gui::EWords 수치 = { { 277, 322 },"shame" };
	Gui::EWords 겁나 = { { 278, 365 },"scare" };
	Gui::EWords 섬뜩 = { { 278, 223 },"creepy" };
	Gui::EWords 질투 = { { 261, 243 },"jealousy" };
	Gui::EWords 두렵 = { { 257, 307 },"afraid" };
	Gui::EWords 무섭 = { { 240, 256 },"scare" };
	Gui::EWords 약오 = { { 233, 205 },"provoking" };
	Gui::EWords 실망 = { { 224, 311 },"disappoint" };
	Gui::EWords 억울 = { { 220, 268 },"aggrievement" };
	Gui::EWords 원망 = { { 216, 221 },"resent" };
	Gui::EWords 얄밉 = { { 215, 174 },"detestable" };
	Gui::EWords 격분 = { { 212, 121 },"outrages" };
	Gui::EWords 환멸 = { { 200, 153 },"disillusion" };
	Gui::EWords 불쾌 = { { 199, 194 },"offend" };
	Gui::EWords 반감 = { { 182, 140 },"antipathy" };
	Gui::EWords 짜증 = { { 178, 186 },"annoyance" };
	Gui::EWords 밉다 = { { 168, 129 },"hate" };
	Gui::EWords 증오 = { { 163, 113 },"hatred" };
	Gui::EWords 배신 = { { 164, 201 },"betrayal" };
	Gui::EWords 역겹 = { { 121, 106 },"disgusting" };
	Gui::EWords 경멸 = { { 140, 94 },"contempt" };
}

using namespace EW;

int index = 0;

String pre_text[100];	//brain_typo함수 관련 단어저장소
Gui::brain_data pre_data[100];	//brain_typo함수 관련 data저장소

Mat color_img = imread("img/whale.jpg", IMREAD_COLOR);	//원형색상그래프 이미지 불러오기//Mat color_imgS = Gui::readimg("img/whale.jpg", 380, 380);	//원형색상그래프 이미지 불러오기
Mat stick_img = imread("img/stick.jpg", IMREAD_COLOR);	//막대그래프 이미지 불러오기
Mat emotion_grid = imread("img/emotion_grid.jpg", IMREAD_COLOR);	//emotion_grid 이미지 불러오기
Mat black_bg = imread("img/black_bg.jpg", IMREAD_COLOR);	//검정배경 불러오기
Mat white_bg = imread("img/white_bg.jpg", IMREAD_COLOR);	//검정배경 불러오기

void gui_main(Point circleL_emotion, Point circleS_emotion, int stick_pleasantness, int stick_energy)
{
	Mat circleL = Gui::color_line_chart(color_img, circleL_emotion);	//좌표값에 따라 화살표그려주는 함수(점진적인감정)
	Mat circleS = Gui::color_line_chart(color_img, circleS_emotion);	//좌표값에 따라 화살표그려주는 함수(즉각적인감정)
	Mat stick = Gui::stick_chart(stick_img, stick_pleasantness, stick_energy);		//좌표값을 막대그래프로 나타내주는 함수
	
	Gui::combine_imshow(circleL, circleS, stick);
}

void brain_typo(Gui::EWords text, float hue, int transparency, Point point, int font_scale)
{
	int flag = 0;//flag초기화

	Mat img = white_bg;	//기본배경 설정
	Gui::brain_data data = { img,text,hue,transparency,point,font_scale }; //입력받은 값 data변수에 담기

	if (index == 0)
	{	//처음값은 모두 data,text값 모두 저장
		pre_data[index] = Gui::whale_brain(data);
		pre_text[index] = data.text.text;
		index++;
	}
	else
	{
		//기존 단어들과 비교하여 이미 있는 단어면 기존단어data에 새로운 data덮기
		for (int i = 0; i < index; i++)
		{
			if (pre_text[i] == data.text.text)
			{
				pre_data[i] = Gui::whale_brain(data);
				flag = 1;
				//printf("already\n");
			}
		}
		if (flag == 0)//기존에 없는 단어면 data,text저장
		{
			pre_data[index] = Gui::whale_brain(data);
			pre_text[index] = data.text.text;
			//printf("not yet\n");
			index++;
		}
	}

	//img출력을 위한 for문
	for (int i = 0; i < index; i++)
	{
		if (i == 0)
		{
			//첫index img불러오기
			img = pre_data[i].img;
		}
		else
		{
			//단어 추가해서 img생성
			pre_data[i].img = img;
			pre_data[i] = Gui::whale_brain(pre_data[i]);
			img = pre_data[i].img;
		}
	}
	imshow("whale brain", img);
	//waitKey(500);
}

//이미지파일 불러오고 사이즈 조절하는 함수
Mat Gui::readimg(String name, int x, int y)  //(파일명, x축size, y축size)
{
	Mat img = imread(name, IMREAD_COLOR);
	resize(img, img, Size(x, y));

	return img;
}

//감정값 whale이미지에 맞게 비율 맞추는 함수
Point Gui::percent(Point emotion)
{
	emotion *= (379.0 / 100.0);
	
	return emotion;
}

//좌표->각도로 변환하는 함수
float Gui::xy2deg(int x, int y)
{
	float degree = atan2(x, y) * 180 / PI;

	return degree;
}

//각도에따른 색으로 변환해주는 함수
Scalar Gui::deg2hue(float degree)
{
	//좌표각도->hue값으로 변환
	int hue;

	//각도값이 -180~180 범위로 표현되기 때문에 0~360도로 변환하기위한 조건문
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

	//hue값 color로 변환
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

//hsv->rgb로 바꾸는 함수
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

//rgb->hsv로 바꾸는 함수
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

//좌표값에 따라 화살표그려주는 함수
Mat Gui::color_line_chart(Mat img, Point emotion)  //(whale원형그래프이미지, 감정값)
{
	Scalar color;
	Point center, result;
	
	Mat img_circle = img.clone();

	emotion = percent(emotion);

	center.x = img_circle.cols / 2;			//이미지 중심좌표저장
	center.y = img_circle.rows / 2;

	result.x = center.x + emotion.x;				//감정값의 원점을 이미지 중심으로 옮김
	result.y = img_circle.cols - (center.y + emotion.y);	//y좌표의 0점이 화면상단에서 시작하므로 반전시킴

	
	color = deg2hue(xy2deg(emotion.x, emotion.y));			//좌표에 따른 bgr값 받아옴

	circle(img_circle, Point(center.x, center.y), 6, gray, -1);	//그래프 중심
	arrowedLine(img_circle, Point(center.x, center.y), Point(result.x, result.y), color, 5, CV_8UC3, 0, 0.1);	//화살표그리기

	//imshow("grdual_emotion", img_circle);

	return img_circle;
}

//좌표값을 막대그래프로 나타내주는 함수
Mat Gui::stick_chart(Mat& img, int pleasantness, int energy) //(막대그래프이미지, 감정값)
{
	Point result;
	Mat img_stick = img.clone();

	//감정값(-100~100범위를 막대그래프 -180~180범위로 나타내는식
	result.x = (int)(pleasantness * (180.0/100.0));
	result.y = (int)(energy * (180.0 / 100.0));

	if (pleasantness >= 0) //감정의 x좌표(긍정,부정)가 양수이면 초록색으로 막대그래프 채움
	{
		img_stick = draw_rect(img_stick, 250, result.x, green);
	}
	else                //감정의 x좌표(긍정,부정)가 음수이면 보라색으로 막대그래프 채움
	{
		img_stick = draw_rect(img_stick, 250, result.x, purple);
	}

	if (energy >= 0) //감정의 y좌표(에너지)가 양수이면 빨간색으로 막대그래프 채움
	{
		img_stick = draw_rect(img_stick, 420, result.y, red);
	}
	else                //감정의 y좌표(에너지)가 음수이면 파란색으로 막대그래프 채움
	{
		img_stick = draw_rect(img_stick, 420, result.y, blue);
	}

	//imshow("stick_emotion", img_stick);

	return img_stick;
}

//입력받은 감정단어를 emotion_grid에 점찍어주는 함수
Mat Gui::emotion_word_point(Mat& img, Point word) 
{
	Mat img_grid = img.clone();
	
	Scalar color = emotion_word_color(word);	//word의 위치좌표에 맞는 색상을 반환하는 함수
	circle(img_grid, Point(word.x, word.y), 4, color, -1);

	//imshow("emotion_grid", emotion_grid);

	return img_grid;
}

//stick_chart에 사용되는 직사각형 그리기 함수
Mat Gui::draw_rect(Mat img, int p ,int emotion ,Scalar color)
{
	Mat img_clone = img.clone();
	int center_y = 200;	//막대그래프 중심축

	if (emotion >= 0)
		color = trans_color((emotion / 2), color);

	else
		color = trans_color((emotion / 2), color);

	rectangle(img_clone, Point(p+2, center_y), Point(p + 88, center_y - emotion), color, -1);

	return img_clone;
}

//막대바 색 채도 변경 함수
Scalar Gui::trans_color(int emotion, Scalar color)
{
	if (emotion < 0)
	{
		emotion = emotion*(-1);
	}
	double percent = (double)emotion / 100.0;

	percent = (percent / 2 + 0.5);

	//감정정도에 따라 color 채도 변화
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

//text 색 채도 변경 함수
Scalar Gui::trans_text_color(int emotion, Scalar color, int bl_n_wh)
{
	double percent = (double)emotion / 100.0;

	//감정정도에 따라 color 채도 변화
	rgb R;
	hsv P;
	int r, g, b;
	double h, s, v;
	R.b = (double)color[0] / 255.0;
	R.g = (double)color[1] / 255.0;
	R.r = (double)color[2] / 255.0;
	P = Gui::rgb2hsv(R);
	if (bl_n_wh == 1) //흰배경일때
	{
		P.h = P.h;
		P.s = P.s * percent;
		P.v = P.v;
	}
	else             //검은배경일때
	{
		P.h = P.h;
		P.s = P.s;
		P.v = P.v * percent;
	}
	R = Gui::hsv2rgb(P);
	r = (int)(R.r * 255);
	g = (int)(R.g * 255);
	b = (int)(R.b * 255);

	return Scalar(b, g, r);
}

//단어의 좌표 위치에 따른 색상 변환함수
Scalar Gui::emotion_word_color(Point word)
{
	Point word_color = word;	//word_color은 deg2hue함수를 사용하기 위한 좌표값을 위한 변수

	//word값을 emotion값으로 변화시켜주는 계산식
	word_color.x -= zeroP.x;
	word_color.y -= zeroP.y;
	word_color.y = word_color.y * (-1);

	//printf("after: %d, %d\n", word_color.x, word_color.y);
	return deg2hue(xy2deg(word_color.x, word_color.y)); 
}

//고래 뇌속 텍스트를 나타내는 함수
Gui::brain_data Gui::whale_brain(brain_data data)
{
	Mat brain = data.img.clone();
	String text = data.text.text;

	float hue = 450 - (data.hue * 2);	//360 - (hue*2 - 90) 그래프의 색상이 hue값변화의 반대방향에 90도 회전하여 나타내있어 보정하는 계산
	if (hue > 360)
		hue -= 360;
	Scalar color = deg2hue(hue);

	color = trans_text_color(data.transparency, color, 1);//투명도(0~100),색,(검은배경=0,흰배경=1)

	//중심좌표로 나타내는 식
	Point p = data.point;
	p *= 3;
	p.x += 300;
	p.y = 600 - (p.y + 300);

	int font = FONT_HERSHEY_SIMPLEX;  // normal size sans-serif font
	double scale = (double)data.font_scale / 10.0;
	double fontScale = scale;
	int thickness = (int)scale + 1;
	int baseLine;
	Size size = getTextSize(text, font, fontScale, thickness, &baseLine);	//text사이즈계산 함수
	Point word_center;	//text의 중심좌표를 word좌표와 일치시키기위한 계산식
	word_center.x = p.x - (size.width / 2);
	word_center.y = p.y + (size.height / 2);

	putText(brain, text, word_center, font, fontScale, color, thickness);
	data.img = brain;
	//imshow("whale brain", brain);

	return data;
}

//세가지의 창을 하나로 합치는 함수
void Gui::combine_imshow(Mat circleL, Mat circleS, Mat stick)
{
	Mat vertical, horizontal;

	resize(circleL, circleL, Size(600,600));
	resize(circleS, circleS, Size(380,380));
	resize(stick, stick, Size(380, 220));

	vconcat(circleS, stick, vertical);
	hconcat(circleL, vertical, horizontal);
	
	imshow("result", horizontal);
}
