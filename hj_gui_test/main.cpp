/*감정의 변화를 원형그래프와 막대그래프로 나타내는 코드
  추후에 감정엔진으로부터 값을 받아와 표현하는 걸로 수정할것*/

#include "gui_test.h"	//gui와 관련된 헤더파일

int main()
{
	Mat color_img, stick_img;
	Point emotion;

	color_img = Gui::readimg("whale.jpg", 384, 384);	//원형색상그래프 이미지 불러오기
	stick_img = Gui::readimg("stick.jpg", 190, 220);	//막대그래프 이미지 불러오기
	
	printf("변화량을 입력 : ");					
	scanf_s("%d %d", &emotion.x, &emotion.y);	//입력값 emotion에 저장

	Gui::color_line_chart(color_img, emotion);	//좌표값에 따라 화살표그려주는 함수
	Gui::stick_chart(stick_img, emotion);		//좌표값을 막대그래프로 나타내주는 함수
	waitKey(0);
	
}

Mat Gui::readimg(String name, int x, int y) //이미지파일 불러오고 사이즈 조절하는 함수
{
	Mat img;

	img = imread(name, IMREAD_COLOR);
	resize(img, img, Size(x, y));

	return img;
}

Scalar Gui::deg2hue(int x, int y)//좌표값 받아서 각도에따른 색으로 변환해주는 함수
{
	//좌표각도->hue값으로 변환
	float degree;
	int hue;

	degree = atan2(x, y) * 180 / PI; 

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

rgb Gui::hsv2rgb(hsv in)//hsv->rgb로 바꾸는 함수
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

void Gui::color_line_chart(Mat img, Point emotion) //좌표값에 따라 화살표그려주는 함수
{
	int hue;
	Scalar color;
	Point center, result;

	center.x = img.rows / 2;			//이미지 중심좌표저장
	center.y = img.cols / 2;			
	result.x = center.x + emotion.x;				//감정값의 원점을 이미지 중심으로 옮김
	result.y = img.cols - (center.y + emotion.y);	//y좌표의 0점이 화면상단에서 시작하므로 반전시킴

	color = deg2hue(emotion.x, emotion.y);			//좌표에 따른 bgr값 받아옴

	circle(img, Point(center.x, center.y), 4, gray, -1);	//그래프 중심
	line(img, Point(39, 192), Point(348, 192), gray, 1);	//x축
	line(img, Point(192, 38), Point(192, 345), gray, 1);	//y축
	arrowedLine(img, Point(center.x, center.y), Point(result.x, result.y), color, 2, CV_8UC3, 0, 0.1);	//화살표그리기
	
	imshow("grdual_emotion", img);
}

void Gui::stick_chart(Mat img, Point emotion) //좌표값을 막대그래프로 나타내주는 함수
{
	int center_y = 100; //막대그래프 중심축

	if (emotion.x >= 0) //감정의 x좌표(긍정,부정)가 양수이면 초록색으로 막대그래프 채움
	{
		rectangle(img, Point(31, center_y), Point(74, center_y - emotion.x), green, -1);
	}
	else                //감정의 x좌표(긍정,부정)가 음수이면 보라색으로 막대그래프 채움
		rectangle(img, Point(31, center_y), Point(74, center_y - emotion.x), purple, -1);
	
	if (emotion.y >= 0) //감정의 y좌표(에너지)가 양수이면 빨간색으로 막대그래프 채움
	{
		rectangle(img, Point(116, center_y), Point(159, center_y - emotion.y), red, -1);
	}
	else                //감정의 y좌표(에너지)가 음수이면 파란색으로 막대그래프 채움
		rectangle(img, Point(116, center_y), Point(159, center_y - emotion.y), blue, -1);
	
	imshow("stick_emotion", img);
}