#include "bullet.h"
void Bullet::UpdateBullet(){
    for (int i = 0; i < 50; i++) {
		if (bullets[i].flag) {
			bullets[i].x += bullets[i].speed;
			if (bullets[i].x > width) {
				bullets[i].flag = 0;
			}
			//��Ҫ������ײ���
			if (bullets[i].blast) {
				bullets[i].frameindex++;
				if (bullets[i].frameindex > 3) {
					bullets[i].flag = 0;
				}
			}
		}
	}
}

void Bullet::GetImaged()
{
	if(!online)return;
	if(blast){
		IMAGE* img = &im[BLAST_IMAGE_INDEX].datas[frameindex];
		putimagePNG(x, y, img);
	}else {
		putimagePNG(x, y, &im[NORM_IMAGE_INDEX].datas[frameindex]);
	}
}
