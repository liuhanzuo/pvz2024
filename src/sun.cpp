#include"sun.h"
void imagesunshine() {
	for (int i = 0; i < ballmax; i++) {
		if (ball[i].flag) {
			ball[i].time++;
			putimagePNG(ball[i].x, ball[i].y, &balls[ball[i].frameindex]);
			if (ball[i].time >= 200) {
				ball[i].flag = 0;
				ball[i].time = 0;
			}
		}
	}
}
void collectsunshine(ExMessage* msg) {
	int w = balls[0].getwidth();
	int h = balls[0].getheight();
	for (int i = 0; i < ballmax; i++) {
		if (ball[i].flag && !ball[i].collect && (ball[i].x - msg->x + w / 2) * (ball[i].x - msg->x + w / 2)
			+ (ball[i].y - msg->y + h / 2) * (ball[i].y - msg->y + h / 2) <= (w * w + h * h) / 4) {
			ball[i].collect = 1;
			ball[i].startx = ball[i].x;
			ball[i].starty = ball[i].y;
		}
	}
}
void createsun(int x, int y, int created) {
	int i = 0;
	while (i < ballmax && ball[i].flag)
		i++;
	if (i >= ballmax)
		return;
	ball[i].flag = 1;
	ball[i].frameindex = 0;
	ball[i].x = x;
	ball[i].y = y;
	ball[i].desty = 200 + (rand() % 4) * 90;
	ball[i].createdby = created;
}
void createsunshine() {
	static int count = 0;
	static int fre = 50;
	count++;
	if (count < fre) {
		return;
	}
	count = 0;
	fre = 50 + rand() % 50;
	int x = 260 - 112 + rand() % (850 - 260);
	int y = 80;
	createsun(x, y, 0);
}
void sunflowercreatesunshine() {
	for (int i = 0; i < rowl; i++) {
		for (int j = 0; j < columnl; j++) {
			if (zw[i][j].type == 2) {
				zw[i][j].timer++;
				if (zw[i][j].timer < 150) {
					continue;
				}
				zw[i][j].timer = 0;
				int zwx = j * 81 + 256;
				int zwy = i * 102 + 179;
				createsun(zwx, zwy, 1);
			}
		}
	}
}
void updatesunshine() {
	for (int i = 0; i < ballmax; i++) {
		if (ball[i].flag) {
			if (ball[i].collect) {
				//ball[i].flag = 0;
				//ball[i].collect = 0;
				//cursun += 25;
				static int time0 = 0;
				if (time0 >= 10) {
					time0 = 0;
					ball[i].collect = 0;
					ball[i].flag = 0;
					ball[i].startx = 0;
					ball[i].starty = 0;
					cursun += 25;
					//mciSendString("play images/sunshine.mp3", 0, 0, 0);
					break;
				}
				time0++;
				ball[i].x = ball[i].startx - (ball[i].startx - 262) / 10 * time0;
				ball[i].y = ball[i].starty - (ball[i].starty - 0) / 10 * time0;
				continue;
			}
			ball[i].frameindex++;
			ball[i].frameindex %= 29;
			if (ball[i].createdby) {
				continue;
			}
			if (ball[i].y < ball[i].desty) {
				ball[i].y += 2;
			}
		}
	}
	sunflowercreatesunshine();
}