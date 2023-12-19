#include "sunflower.h"
void SunFlower::MakeSun(){
	timer ++ ;
		if (timer < 150) {
			continue;
		}
		timer = 0;
		int zwx = y * 81 + 256;
		int zwy = x * 102 + 179;
		back->CreateSingleSun(zwx, zwy, 1);
}