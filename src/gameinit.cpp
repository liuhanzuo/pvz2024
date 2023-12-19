#include"gameinit.h"
void gameInit() {
	loadimage(&imgbg, "images/bg.jpg");
	loadimage(&imgbar, "images/bar4.png");
	memset(imgzhiwu, 0, sizeof(imgzhiwu));
	memset(zw, 0, sizeof(zw));
	char name[64];
	for (int i = 0; i < cnt; i++) {
		sprintf_s(name, sizeof(name), "images/Cards/card_%d.png", i + 1);
		loadimage(&imgcards[i], name);
		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "images/zhiwu/%d/%d.png", i, j + 1);
			if (fileexist(name)) {
				imgzhiwu[i][j] = new IMAGE;
				loadimage(imgzhiwu[i][j], name);
			}
			else {
				break;
			}
		}
	}
	needsun[1] = 100;
	needsun[2] = 50;
	memset(ball, 0, sizeof(ball));
	for (int i = 1; i <= 29; i++) {
		sprintf_s(name, sizeof(name), "images/sunshine/%d.png", i);
		loadimage(&balls[i - 1], name);
	}
	memset(zms, 0, sizeof(zms));
	for (int i = 1; i <= 22; i++) {
		sprintf_s(name, sizeof(name), "images/zm/%d.png", i);
		loadimage(&z[i - 1], name);
	}
	memset(bullets, 0, sizeof(bullets));
	loadimage(&bulletn, "images/bullets/bullet_normal.png");
	loadimage(&imgballblast[3], "images/bullets/bullet_blast.png");
	for (int i = 0; i < 3; i++) {
		float k = (i + 1) * 0; 2;
		loadimage(&imgballblast[i], "images/bulltes/bullet_blast.png", imgballblast[3].getwidth() * k, imgballblast[3].getheight() * k);
	}
	for (int i = 0; i < 20; i++) {
		sprintf_s(name, sizeof(name), "images/zm_dead/%d.png", i + 1);
		loadimage(&zmdied[i], name);
	}
	for (int i = 0; i < 21; i++) {
		sprintf_s(name, sizeof(name), "images/zm_eat/%d.png", i + 1);
		loadimage(&zmseat[i], name);
	}
	for (int i = 0; i < 11; i++) {
		sprintf_s(name, sizeof(name), "images/zm_stand/%d.png", i + 1);
		loadimage(&zmsstand[i], name);
	}
	srand(time(NULL));
	initgraph(width, height, 1);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	strcpy(f.lfFaceName, "Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setcolor(BLACK);
}