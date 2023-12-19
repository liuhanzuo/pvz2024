#include "peashooter.h"
void PeaShooter::Shoot(){
    int lines[3] = {0};
	int zmcnt = 50 ;
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
							int zwx = 256 -112 + j * 81;
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