#ifndef __PLANT_H__
#define __PLANT_H__
#include<cstdio>
#include<graphics.h>
#include<time.h>
#include<mmsystem.h>
#include<Windows.h>
bool fileexist(const char* name);
void _putimagePNG(int  picture_x, int picture_y, IMAGE* picture);
void putimagePNG(int x, int y, IMAGE* picture);
int getdelay();
#pragma comment(lib,"winmm.lib")
const int width = 900;
const int height = 600;
const int rowl = 3;
const int columnl = 9;
const int ballmax = 10;
int cnt = 3, startx = 325, dis = 65, starty = 2;
int curx, cury, curzhiwu = 0;//0:没选中；k：第k种
int zmsamount = 25, zmscreate = 0, zmsdied = 0;
static int status = 0;
struct plant {
	int type = 0;//0 refers to no plants, and positive integeres refer to rank
	int frameindex = 0;//帧数序号
	int eat;
	int hp;
	int timer;
}zw[rowl][columnl];
int needsun[10];
struct sunshine {
	int x, y;
	int frameindex;
	int desty;//目标位置y坐标
	int flag;//是否在显示
	int time;
	int startx;
	int starty;
	float xoff;
	float yoff;
	int collect;
	int createdby;
}ball[ballmax];
IMAGE balls[30];
int cursun = 150;
//阳光池，把所有可能的路径放在池子里，每次拿一个出来用
struct zm {
	int x;
	int y;
	int frameindex;
	int speed;
	int flag;
	int row;
	int hp;
	int died;
	int eat;
}zms[50];
IMAGE z[25];
IMAGE zmdied[20];
IMAGE zmseat[21];
IMAGE zmsstand[15];
struct bullet {
	int x;
	int y;
	int row;
	int speed;
	int flag;
	int blast;//是否发生爆炸
	int frameindex;//帧序号
}bullets[50];
IMAGE bulletn, bulletb;
IMAGE imgballblast[4];
using namespace std;
//游戏初始化
IMAGE imgbg, imgbar, imgcards[16], * imgzhiwu[16][32];
#endif