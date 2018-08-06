#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include<string.h>
#include <graphics.h> 
#include "mmsystem.h" 
#pragma comment (lib, "winmm.lib")
#define PICPATH				"C:/users/wang/desktop/pic.jpg"
#define THUNDERSOUND		"C:/users/wang/desktop/thunder.mp3"
#define currentDetail 2

int rainDropNumber = 200;
struct rainDrop *head;
MOUSEMSG m,musicm;
void thunder();
void setMusic();

struct rainCircle
{
	int x;
	int y;
	int r;
	int currentR;
	int rainCircleStep;
};

struct rainDrop
{
	int startX;
	int startY;
	int endY;
	int currentX;
	int currentY;
	int rainColor;
	int rainLineStep;
	int rainLineLength;
	int status;
	struct rainCircle water;
	struct rainDrop *next;
};

void initgraph()
{
	initgraph(640, 480);
	setbkcolor(BLACK);
	cleardevice();
	loadimage(NULL, _T(PICPATH));
}

struct rainDrop *creatDrop()
{
	struct rainDrop *p;
	p = (struct rainDrop *)malloc(sizeof(struct rainDrop));
	p->startX = rand() % 640;
	p->startY = 115+rand() % 430;
	p->endY = 430 + rand() % 50;
	p->currentX = p->startX;
	p->currentY = p->startY;
	p->rainLineStep = 8;
	int c = rand() % 256;
	int d = rand() % 256;
	int e = rand() % 256;
	COLORREF color RGB(c, d, e);
	p->rainColor = color;
	p->rainLineLength = 1;
	p->status = 0;
	p->water.x = p->startX;
	p->water.y = p->endY;
	p->water.r = rand() % 40;
	p->water.currentR = rand() % 2;
	p->water.rainCircleStep = rand() % 2 + 1;
	p->next = NULL;
	return(p);
}

void recreatDrop(struct rainDrop *p)
{
	p->startX = rand() % 640;
	p->startY = 115+rand() % 30;
	p->endY = 430 + rand() % 50;
	//p->endY = 430 + rand() % 50;
	p->currentX = p->startX;
	p->currentY = p->startY;
	p->rainLineStep = 8;
	int c = rand() % 256;
	int d = rand() % 256;
	int e = rand() % 256;
	COLORREF color RGB(c, d, e);
	p->rainColor =color; /*rand() % 256*/
	p->rainLineLength = rand() % 10 + 0;
	p->status = 0;
	p->water.x = p->startX;
	p->water.y = p->endY;
	p->water.r = rand() % 40;
	p->water.currentR = rand() % 2;
	p->water.rainCircleStep = rand() % 2 + 1;
}

void creatRain()
{
	struct rainDrop *p1, *p2;
	int i;
	p1 = p2 = creatDrop();
	head = p1;
	
	for (i = 0; i<rainDropNumber; i++)
	{
		p2 = creatDrop();
		p1->next = p2;
		p1 = p2;
	}
}

void updateRainLineData(struct rainDrop *p)
{
	if (MouseHit())
		m = GetMouseMsg();
	p->currentY += p->rainLineStep;//Y的增量为rainLineStep
	if (m.y < 120)
	{
		p->currentY -= p->rainLineStep / 2;
		settextcolor(WHITE);
		RECT r = { 0, 0, 100, 90 };
		drawtext(_T("小雨"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	if (m.y >120&&m.y<240)
	{
		p->currentY -= p->rainLineStep / 8;
		settextcolor(WHITE);
		RECT r = { 0, 0, 100, 90 };
		drawtext(_T("中雨"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	if (m.y > 240&&m.y<360)
	{
		p->currentY += p->rainLineStep * 2;
		settextcolor(WHITE);
		RECT r = { 0, 0, 100, 90 };
		drawtext(_T("大雨"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	if (m.y > 360)
	{
		p->currentY += p->rainLineStep * 4;
		settextcolor(WHITE);
		RECT r = { 0, 0, 100, 90 };
		drawtext(_T("暴雨"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	p->startY = p->currentY - p->rainLineLength;
	if (m.x < 128)
	{
		p->currentX -= p->rainLineStep*3;
		p->startX = p->currentX + p->rainLineLength;
		settextcolor(WHITE);
		RECT r = { 0, 0, 180, 90 };
		drawtext(_T("大风"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	if (m.x > 128&&m.x<256)
	{
		p->currentX -= p->rainLineStep*1;
		p->startX = p->currentX + p->rainLineLength;
		settextcolor(WHITE);
		RECT r = { 0, 0, 180, 90 };
		drawtext(_T("微风"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	if (m.x > 256 && m.x<384)
	{
		
		settextcolor(WHITE);
		RECT r = { 0, 0, 180, 90 };
		drawtext(_T("无风"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	if (m.x > 384 && m.x<512)
	{
		p->currentX += p->rainLineStep * 1;
		p->startX = p->currentX - p->rainLineLength;
		settextcolor(WHITE);
		RECT r = { 0, 0, 180, 90 };
		drawtext(_T("微风"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	if (m.x >512)
	{
		p->currentX += p->rainLineStep * 3;
		p->startX = p->currentX - p->rainLineLength;
		settextcolor(WHITE);
		RECT r = { 0, 0, 180, 90 };
		drawtext(_T("大风"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	setMusic();
}

void drawRainLine(struct rainDrop *p)
{
	setlinestyle(PS_DOT,3);
	setlinecolor(p->rainColor);
	line(p->startX, p->startY, p->currentX, p->currentY);
	setlinestyle(PS_SOLID, 1);
}

void clearRainLine(struct rainDrop *p)
{
	if (p->startY > 395)
		setcolor(BLUE);
	else
		setcolor(BLACK);
	//setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	line(p->startX, p->startY, p->currentX, p->currentY);
	//loadimage(NULL, _T("C:\\Users\\Wang\\Desktop\\pic.jpg"));
	setlinestyle(PS_SOLID, 1);

}

void rainDropDown(struct rainDrop *p)
{
	if (p->currentY >= p->endY)
	{
		clearRainLine(p);
		p->status = 1;
	}
	else
	{
		clearRainLine(p);
		updateRainLineData(p);
		drawRainLine(p);
	if (m.mkLButton)
		thunder();
	}
}

void updateRainCircleData(struct rainDrop *p)
{
	p->water.currentR += p->water.rainCircleStep;
}

void drawRainCircle(struct rainDrop *p)
{
	setcolor(p->rainColor);
	int x1, x2, y1, y2;
	x1 = p->water.x - p->water.currentR - 5;
	x2 = p->water.x + p->water.currentR + 5;
	y1 = p->water.y - p->water.currentR + 5;
	y2 = p->water.y + p->water.currentR - 5;
	ellipse(x1, y1, x2, y2);
}

void clearRainCircle(struct rainDrop *p)
{
	setlinecolor(BLUE);
	int x1, x2, y1, y2;
	x1 = p->water.x - p->water.currentR - 5;
	x2 = p->water.x + p->water.currentR + 5;
	y1 = p->water.y - p->water.currentR + 5;
	y2 = p->water.y + p->water.currentR - 5;
	ellipse(x1, y1, x2, y2);
}

void fallToWater(struct rainDrop *p)
{
	if (p->water.currentR >= p->water.r)
	{
		clearRainCircle(p);
		recreatDrop(p);
	}
	else
	{
		clearRainCircle(p);
		updateRainCircleData(p);
		drawRainCircle(p);
	}
}

void setMusic()
{
	//while (1)
	//{
		//if (MouseHit())
		if (m.mkLButton)
			mciSendString(_T("play " THUNDERSOUND), NULL, 0, NULL);
		/*if (musicm.y < 120)
		{
		//	mciSendString(_T("close all"), NULL, 0, NULL);
			mciSendString(_T("play " SMALLRAINSOUND), NULL, 0, NULL);
		}
		if (musicm.y > 120 && m.y < 240)
		{
		//	mciSendString(_T("close all"), NULL, 0, NULL);
			mciSendString(_T("play " MEDIUMRAINSOUND), NULL, 0, NULL);
		}
		if (musicm.y > 240 && m.y < 360)
		{
		//	mciSendString(_T("close all"), NULL, 0, NULL);
			mciSendString(_T("play " BIGRAINSOUND), NULL, 0, NULL);
		}
		if (musicm.y > 360)
		{
		//	mciSendString(_T("close all"), NULL, 0, NULL);
			mciSendString(_T("play " SUPERRAINSOUND), NULL, 0, NULL);
		}
	}*/
}

void rain()
{
	

	struct rainDrop *p;
	p = head;
	while (!_kbhit())
	{
		if (p == NULL)
			p = head;
		else
		{
			if (p->status == 0)
				rainDropDown(p);
			else
				fallToWater(p);
		}
		p = p->next;
	}
}

void drawLightning(double x1, double y1, double x2, double y2, double displace)
{
	//当被掐弯的线段达到一个最小值时，画出来
	if (displace < currentDetail) {
		line(x1, y1, x2, y2);
	}
	else {
		double mid_x = (x2 + x1) / 2;
		double mid_y = (y2 + y1) / 2;//算一条线段的中间位置
		mid_x += (double)((rand() % 10) / 10.0 - 0.5)*displace;//(double)((rand()%10)/10.0-0.5)相当于从[-0.5,0.5]随机取数
		mid_y += (double)((rand() % 10) / 10.0 - 0.5)*displace;//改变这条线段中间的位置，使其歪成两半
		drawLightning(x1, y1, mid_x, mid_y, displace / 2);////
		drawLightning(x2, y2, mid_x, mid_y, displace / 2);//继续对这两半进行随机掰弯操作
	}

}

void thunder()
{
	//setMusic();
	settextcolor(WHITE);
	RECT r = { 0, 0, 260, 90 };
	drawtext(_T("打雷"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	int thunderNumber = rand() % 5 + 1;
	if (thunderNumber > 5)
		thunderNumber = 5;
	for (int i = 0; i < thunderNumber; i++)
	{
		int x1, x2;
		x1 = (rand() % 640);
		x2 = (rand() % 640);
		setlinecolor(WHITE);
		for (int i = 0; i <= 3; i++)
		drawLightning(x1, 105, x2, 480, 200);
		Sleep(50);
		setfillcolor(WHITE);
		bar(0, 100, 640, 480);
		Sleep(50);
		setfillcolor(BLACK);
		bar(0, 100, 640, 480);
	}
	loadimage(NULL, _T(PICPATH));
	setfillcolor(BLACK);
	bar(0, 100, 640, 400);
	setfillcolor(BLUE);
	bar(0, 400, 640, 480);
	settextcolor(WHITE);
	drawtext(_T(""), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void pond()
{
	//setlinecolor(BLUE);
	//line(0, 420, 640, 420);
	setfillstyle(0, NULL, NULL);
	setfillcolor(BLUE);
	bar(0, 400, 640, 480);
	//池塘
}

void clearAll(void)
{
	struct rainDrop *p, *pf;
	p = head;
	while (p != NULL)
	{
		pf = p;
		p = p->next;
		free(pf);
	}
	closegraph();
}
void main()
{
	initgraph();
	pond();
	creatRain();	
	rain();
	clearAll();
}