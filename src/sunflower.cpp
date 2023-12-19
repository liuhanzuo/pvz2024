#include "sunflower.h"
void SunFlower::MakeSun(){
    for (int i = 0; i < rowl; i++) {
	    for (int j = 0; j < columnl; j++) {
			if (zw[i][j].type == 2) {
				zw[i][j].timer ++ ;
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