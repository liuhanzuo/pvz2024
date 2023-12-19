#include"collision.h"

void imagebullet() {
	for (int i = 0; i < 50; i++) {
		if (!bullets[i].flag) {
			continue;
		}
		if (bullets[i].blast) {
			IMAGE* img = &imgballblast[bullets[i].frameindex];
			putimagePNG(bullets[i].x, bullets[i].y, img);
		}
		else {
			putimagePNG(bullets[i].x, bullets[i].y, &bulletn);
		}
	}
}
void shoot() {
	int lines[3] = { 0 };
	int zmcnt = 50;
	int bulletmax = 50;
	int danger = 900;
	int fre = 20;
	for (int i = 0; i < zmcnt; i++) {
		if (zms[i].flag && zms[i].x < danger) {
			lines[zms[i].row] = 1;
		}
	}
	static int amt = 1;
	if (GetAsyncKeyState(VK_SPACE)) {
		amt = 2;
	}
	static int count[3][9] = { 0 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (zw[i][j].type == 1 && lines[i]) {
				count[i][j]++;
				if (count[i][j] > fre) {
					count[i][j] = 0;
					int k = 0;
					for (int ccc = 1; ccc <= amt; ccc++) {
						while (k < bulletmax && bullets[k].flag) {
							k++;
						}
						if (k < bulletmax) {
							bullets[k].flag = 1;
							bullets[k].row = i;
							bullets[k].speed = 6;
							bullets[k].blast = 0;
							bullets[k].frameindex = 0;
							int zwx = 256 - 112 + j * 81;
							int zwy = 179 + i * 102 + 14;
							bullets[k].x = zwx + imgzhiwu[zw[i][j].type - 1][0]->getwidth() - 10 + (ccc - 1) % 2 * 25;
							bullets[k].y = zwy + 5;
						}
					}
				}
			}
			else {
				count[i][j] = 0;
			}
		}
	}
}
void updatebullet() {
	for (int i = 0; i < 50; i++) {
		if (bullets[i].flag) {
			bullets[i].x += bullets[i].speed;
			if (bullets[i].x > width) {
				bullets[i].flag = 0;
			}
			if (bullets[i].blast) {
				bullets[i].frameindex++;
				if (bullets[i].frameindex > 3) {
					bullets[i].flag = 0;
				}
			}
		}
	}
}
void collisionbullet2zm() {
	int bulletmax = 50;
	int zmsmax = 50;
	for (int i = 0; i < bulletmax; i++) {
		if (!bullets[i].flag || bullets[i].blast) {
			continue;
		}
		for (int k = 0; k < zmsmax; k++) {
			if (!zms[k].flag || zms[k].died) {
				continue;
			}
			int x1 = zms[k].x + 75;
			int x2 = zms[k].x + 115;
			if (bullets[i].x > x1 && bullets[i].x < x2 && bullets[i].row == zms[k].row) {
				zms[k].hp -= 15;
				if (zms[k].hp <= 0) {
					zms[k].hp = 0;
					zms[k].died = 1;
					zms[k].frameindex = 0;
				}
				bullets[i].blast = 1;
				bullets[i].speed = 0;
				break;
			}
		}
	}
}
void collisionzm2zw() {
	int zmcnt = 50;
	for (int i = 0; i < 50; i++) {
		if (zms[i].died || !zms[i].flag)
			continue;
		int row = zms[i].row;
		for (int k = 0; k < 9; k++) {
			if (zw[row][k].type == 0)
				continue;
			int zwx = 256 - 112 + k * 81;
			int x1 = zwx + 10;
			int x2 = zwx + 60;
			int x3 = zms[i].x + 80;
			if (x3 > x1 && x3 < x2) {
				if (zms[i].eat) {
					zw[row][k].hp -= 2;
				}
				else {
					zw[row][k].eat = 1;
					printf("a\n");
					zms[i].speed = 0;
					zms[i].eat = 1;
				}
				if (zw[row][k].hp <= 0) {
					zw[row][k].type = 0;
					zw[row][k].hp = 0;
					zw[row][k].eat = 0;
					zw[row][k].frameindex = 0;
					zms[i].speed = 2 + rand() % 3;
					zms[i].eat = 0;
					zms[i].frameindex = 0;
				}
			}
		}
	}
}
void collisioncheck() {
	collisionbullet2zm();
	collisionzm2zw();
}