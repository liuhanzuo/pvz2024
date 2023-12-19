#include"game.h"
void updateimage() {
	BeginBatchDraw();
	putimage(-112, 0, &imgbg);
	putimage(250, 0, &imgbar);
	for (int i = 0; i < cnt; i++) {
		int x = startx + i * dis;
		int y = starty;
		putimage(x, y, &imgcards[i]);
	}
	for (int i = 0; i < rowl; i++) {
		for (int j = 0; j < columnl; j++) {
			int x = 256 - 112 + j * 81;
			int y = 179 + i * 102;
			int tp = zw[i][j].type;
			IMAGE* img = imgzhiwu[zw[i][j].type - 1][zw[i][j].frameindex];
			if (tp && tp != 3) {
				putimagePNG(x, y, img);
			}
			else if (tp == 3) {
				putimagePNG(x, y - 25, img);
			}
		}
	}
	if (curzhiwu) {
		IMAGE* img = imgzhiwu[curzhiwu - 1][0];
		putimagePNG(curx - img->getwidth() / 2, cury - img->getheight() / 2, img);
	}
	imagesunshine();
	imagezombies();
	imagebullet();
	char text[16];
	sprintf_s(text, sizeof(text), "%d", cursun);
	outtextxy(276, 55, text);
	EndBatchDraw();
}
bool win() {
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
void userclick() {
	ExMessage msg;
	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			if (msg.x >= startx && msg.x < startx + cnt * dis && msg.y < 96) {
				int index = (msg.x - 338) / 65;
				if (cursun >= needsun[index + 1]) {
					status = 1;
					curzhiwu = index + 1;
				}
			}
			collectsunshine(&msg);
		}
		else if (msg.message == WM_MOUSEMOVE && status == 1) {
			curx = msg.x;
			cury = msg.y;
		}
		else if (msg.message = WM_LBUTTONUP && status == 1) {
			if (msg.x > 256 - 112 && msg.y > 179 && msg.y < 489) {
				int row = (msg.y - 179) / 102;
				int col = (msg.x - 256 + 112) / 81;
				if (zw[row][col].type == 0) {
					zw[row][col].type = curzhiwu;
					zw[row][col].frameindex = 0;
					zw[row][col].hp = 100;
					cursun -= needsun[curzhiwu];
				}
			}
			curzhiwu = 0;
			status = 0;
		}
		int row = (msg.y - 179) / 102;
		int col = (msg.x - 256 + 112) / 81;
		if ((GetAsyncKeyState('Q') & 0x8000) && zw[row][col].type) {
			zw[row][col].type = 0;
		}
	}
}
void updategame() {
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
void start() {
	IMAGE imgbg0, imgbg1, imgbg2;
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
void ready() {
	IMAGE r[4];
	loadimage(&imgbg, "images/bg.jpg");
	loadimage(&r[1], "images/StartReady.png");
	loadimage(&r[2], "images/StartSet.png");
	loadimage(&r[3], "images/StartPlant.png");
	putimage(-112, 0, &imgbg);
	int qwq = 0;
	for (int i = 1; i <= 3; i++) {
		while (qwq <= 600) {
			qwq += getdelay();
			putimage(400, 200, &r[i]);
		}
		qwq = 0;
	}
}
void view() {
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