#include "game.h"

Game::Game(){
	
}

void Game::Init() {
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
	needsun[3] = 150;
	needsun[4] = 50;
	memset(ball, 0, sizeof(ball));
	for (int i = 1; i <= 29; i++) {
		sprintf_s(name, sizeof(name), "images/sunshine/%d.png", i);
		loadimage(&balls[i-1], name);
	}
	memset(zms, 0, sizeof(zms));
	for (int i = 1; i <= 22; i++) {
		sprintf_s(name, sizeof(name), "images/zm/%d.png", i);
		loadimage(&z[i-1], name);
	}
	memset(bullets, 0, sizeof(bullets));
	loadimage(&bulletn, "images/bullets/bullet_normal.png");
	loadimage(&imgballblast[3], "images/bullets/bullet_blast.png");
	for (int i = 0; i < 3; i++) {
		float k = (i + 1) * 0; 2;
		loadimage(&imgballblast[i], "images/bulltes/bullet_blast.png", imgballblast[3].getwidth() * k, imgballblast[3].getheight()*k);
	}
	for (int i = 0; i < 20; i++) {
		sprintf_s(name,sizeof(name), "images/zm_dead/%d.png", i + 1);
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

bool Game::Win(){
	if (zmsdied >= zmsamount) {
		IMAGE imgwon;
		printf("win\n");
		loadimage(&imgwon, "images/Credits_ZombieNote.png");
		putimagePNG(150, 75, &imgwon);
		mciSendString("play images/win.mp3", 0, 0, 0);
		return false;
	}
	return true;
}

void Game::CreateZombies(){
	if (zmscreate > zmsamount) {
		return;
	}
	static int count = 0;
	static int fr=150;
	static int fre = 150;
	if (GetAsyncKeyState(VK_SPACE)) {
		fr = 30;
	}
	count++;
	if (count < fre) {
		return;
	}
	count = 0;
	fre = fr + rand() % (fr/3);
	int i = 0;
	while (i < 50 && zms[i].flag)
		i++;
	if (i >= 50)
		return;
	zmscreate++;
	zms[i].flag = 1;
	zms[i].frameindex = 0;
	zms[i].x = width;
	zms[i].row = rand() % 3;
	zms[i].y = 140 + zms[i].row * 100;
	zms[i].speed = 2 + rand() % 3;
	zms[i].hp = 100;
}

void Game::UpdateStep(){
	for (int i = 0; i < rowl; i++) {
		for (int j = 0; j < columnl; j++) {
			if (!zw[i][j].type) {
				continue;
			}
			zw[i][j].frameindex++;
			if (imgzhiwu[zw[i][j].type - 1][zw[i][j].frameindex] == NULL)
				zw[i][j].frameindex = 0;
		}
	}
	createsunshine();
	updatesunshine();
	createzombies();
	updatezombies();
	shoot();
	updatebullet();
	collisioncheck();
}

void Game::PlayVideo(){
	int xmin = width - imgbg.getwidth();
	int xs[9] = { 550,530,630,530,515,565,605,705,690 };
	int ys[9] = { 80,160,170,200,270,370,340,280,340 };
	int index[9];
	for (int i = 0; i < 9; i++) {
		index[i] = rand() % 11;
	}
	int count = 0;
	for (int x = 0; x >= xmin; x -= 3) {
		BeginBatchDraw();
		putimage(x, 0, &imgbg);
		count++;
		for (int k = 0; k < 9; k++) {
			putimagePNG(xs[k]-xmin+x,ys[k],&zmsstand[index[k]]);
			if (count >= 10) {
				index[k] = (index[k] + 1) % 11;
			}
		}
		if (count >= 10) {
			count = 0;
		}
		EndBatchDraw();
		Sleep(5);
	}
	//ͣ��1s����
	for (int i = 0; i < 100; i++) {
		BeginBatchDraw();
		putimage(xmin, 0, &imgbg);
		for (int k = 0; k < 9; k++) {
			putimagePNG(xs[k], ys[k], &zmsstand[index[k]]);
			index[k] = (index[k] + 1) % 11;
		}
		EndBatchDraw();
		Sleep(30);
	}
	for (int x = xmin; x <= -112; x += 3) {
		BeginBatchDraw();
		putimage(x, 0, &imgbg);
		count++;
		for (int k = 0; k < 9; k++) {
			putimagePNG(xs[k] - xmin + x, ys[k], &zmsstand[index[k]]);
			if (count >= 10) {
				index[k] = (index[k] + 1) % 11;
			}
		}
		if (count >= 10) {
			count = 0;
		}
		EndBatchDraw();
		Sleep(5);
	}
}


void Game::Start(){
	//���������˵�
	IMAGE imgbg0,imgbg1,imgbg2;
	loadimage(&imgbg0, "images/menu.png");
	loadimage(&imgbg1, "images/menu1.png");
	loadimage(&imgbg2, "images/menu2.png");
	int flag0 = 0;
	while (true) {
		BeginBatchDraw();
		putimage(0, 0, &imgbg0);
		putimagePNG(474, 75, flag0 ? &imgbg2 : &imgbg1);
		EndBatchDraw();
		ExMessage msg;
		if (!peekmessage(&msg)) {
			continue;
		}
		if (msg.x > 474 && msg.x < (474 + 300) 
			&& msg.y > 75 && msg.y < (75 + 140)) {
			flag0 = 1;
		}
		else {
			flag0 = 0;
		}
		if (msg.message == WM_LBUTTONUP && flag0) {
			EndBatchDraw();
			break;
		}
	}
}

void Game::Ready(){
	IMAGE r[4];
	loadimage(&imgbg, "images/bg.jpg");
	loadimage(&r[1], "images/StartReady.png");
	loadimage(&r[2], "images/StartSet.png");
	loadimage(&r[3], "images/StartPlant.png");
	putimage(-112, 0, &imgbg);
	int qwq = 0;
	for (int i = 1; i <= 3; i++) {
		while (qwq <= 600) {
			qwq += GetDelay();
			putimage(400, 200, &r[i]);
		}
		qwq = 0;
	}
}