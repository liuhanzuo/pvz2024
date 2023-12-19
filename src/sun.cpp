#include"sun.h"
void Sun::GetImaged(){
	if (online) {
			time++;
			putimagePNG(x,y, &im[0]->datas[frameindex]);
			if (time >= 200) {
				online = 0;
				time = 0;
			}
	}
}
