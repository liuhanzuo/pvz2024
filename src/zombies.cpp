#include"zombies.h"

void Zombies::GetImaged(){
	if(!online)return;
	if (died) {
		putimagePNG(x, y, &im[DEAD_IMAGE_INDEX].datas[frameindex]);
		continue;
	}
	if (eat) {
		putimagePNG(x, y, &im[EAT_IMAGE_INDEX].datas[frameindex]);
		continue;
	}
	putimagePNG(x, y, &im[WALK_IMAGE_INDEX].datas[frameindex]);
	if (x < 105) {
		IMAGE zmwin;
		loadimage(&zmwin, "images/ZombiesWon.jpg");
		putimage(250, 50, &zmwin);
		mciSendString("play images/lose.mp3", 0, 0, 0);
	}
}
