#include"zombies.h"
void imagezombies() {
	for (int i = 0; i < 50; i++) {
		if (zms[i].flag) {
			if (zms[i].died) {
				putimagePNG(zms[i].x, zms[i].y, &zmdied[zms[i].frameindex]);
				continue;
			}
			if (zms[i].eat) {
				putimagePNG(zms[i].x, zms[i].y, &zmseat[zms[i].frameindex]);
				continue;
			}
			putimagePNG(zms[i].x, zms[i].y, &z[zms[i].frameindex]);
			if (zms[i].x < 105) {
				IMAGE zmwin;
				loadimage(&zmwin, "images/ZombiesWon.jpg");
				putimage(250, 50, &zmwin);
				mciSendString("play images/lose.mp3", 0, 0, 0);
			}
		}
	}
}
void createzombies() {
	if (zmscreate > zmsamount) {
		return;
	}
	static int count = 0;
	static int fr = 150;
	static int fre = 150;
	if (GetAsyncKeyState(VK_SPACE)) {
		fr = 30;
	}
	count++;
	if (count < fre) {
		return;
	}
	count = 0;
	fre = fr + rand() % (fr / 3);
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
void updatezombies() {
	for (int i = 0; i < 50; i++) {
		if (zms[i].flag) {
			if (zms[i].died) {
				zms[i].frameindex++;
				if (zms[i].frameindex >= 20) {
					zms[i].flag = 0;
					zms[i].died = 0;
					zms[i].eat = 0;
					zms[i].frameindex = 0;
					zmsdied++;
					printf("%d\n", zmsdied);
				}
				continue;
			}
			if (zms[i].eat) {
				zms[i].frameindex = (zms[i].frameindex + 1) % 21;
				continue;
			}
			zms[i].frameindex++;
			zms[i].frameindex %= 22;
			if (zms[i].x >= 100)
				zms[i].x -= zms[i].speed;
		}
	}
}