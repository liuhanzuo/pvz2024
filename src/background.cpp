#include "background.h"
int BackGround::GetDelay() const {
	// lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}
bool BackGround::fileexist(const char* name) const{
	FILE* fp = fopen(name, "r");
	if (fp) {
		fclose(fp);
	}
	return fp != NULL;
}
void BackGround::UpdateImage(){
	BeginBatchDraw();
	putimage(-112, 0, &imgbg);
	putimage(250, 0, &imgbar);

	for (int i = 0; i < MAX_PLANT_TYPE; i++) {
		int x = startx + i * dis;
		int y = starty;
		putimage(x, y, &cards.data[i]);
	}
	for (int i = 0; i < rowl; i++) {
		for (int j = 0; j < columnl; j++) {
			int x = 256 -112 + j * 81;
			int y = 179 + i * 102;
			int tp = zw[i][j].type;
			IMAGE* img = Plant::im[tp-1].datas[zw[i][j].frameindex];
			if (tp&&tp!=Plant::MOUTH) {
				putimagePNG(x, y, img);
			}
			else if (tp == Plant::MOUTH) {
				putimagePNG(x, y - 25, img);
			}
		}
	}
	if (curzhiwu) {
		IMAGE* img = Plant::im[curzhiwu-1].datas[0];
		putimagePNG(curx - img->getwidth() / 2, cury - img->getheight() / 2, img);
	}
	for(int i=0;i<ballmax;i++)ball[i].GetImaged();
	for(int i=0;i<zmsmax;i++)zms[i].GetImaged();
	for(int i=0;i<bulletmax;i++)bullets[i].GetImaged();
	char text[16];
	sprintf_s(text, sizeof(text), "%d", cursun);
	outtextxy(276,55,text);
	EndBatchDraw();
}

void BackGround::CollectSun(ExMessage* msg) {
	int w = Sun::im[0].getwidth();
	int h = Sun::im[0].getheight();
	for (int i = 0; i < ballmax; i++) {
		if (ball[i].flag && !ball[i].collect && (ball[i].x - msg->x + w / 2) * (ball[i].x - msg->x + w / 2)
			+ (ball[i].y - msg->y+ h/2) * (ball[i].y - msg->y+ h/2) <= (w*w+h*h)/4) {
			ball[i].collect = 1;
			ball[i].startx = ball[i].x;
			ball[i].starty = ball[i].y;
		}
	}
}

void BackGround::UserClick(){
    ExMessage msg;
	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			if (msg.x >= startx && msg.x < startx + cnt * dis && msg.y < 96 ) {
				int index = (msg.x - 338) / 65;
				if (cursun >= needsun[index + 1]) {
					status = 1;
					curzhiwu = index + 1;
				}
			}
			CollectSun(&msg);
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
		int col = (msg.x - 256 +112) / 81;
		if ((GetAsyncKeyState('Q') & 0x8000) &&zw[row][col].type) {
			zw[row][col].type = 0;
		}
	}
}


void BackGround::CreateSingleSun(int x,int y,int created){
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

void BackGround::MakeSuns(){
	count++;
	if (count < fre) {
		return;
	}
	count = 0;
	fre = 50 + rand() % 50;
	int x = 260 -112 + rand() % (850 - 260);
	int y = 80;
	CreateSingleSun(x ,y, 0);
}

void BackGround::SunFalls(){
    for (int i = 0; i < ballmax; i++) {
		if (ball[i].flag) {
			if (ball[i].collect) {
			    //ball[i].flag = 0;
				//ball[i].collect = 0;
				//cursun += 25;
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
	for (int i = 0; i < rowl; i++) {
	    for (int j = 0; j < columnl; j++) {
			if(zw[i][j].type==Plant::SUN){
				zw[i][j].MakeSun();
			}
		}
	}
}

void BackGround::UpdateZombies(){
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
					printf("%d\n",zmsdied);
				}
				continue;
			}
			if (zms[i].eat) {
				zms[i].frameindex = (zms[i].frameindex + 1) % 21;
				continue;
			}
			zms[i].frameindex++;
			zms[i].frameindex %= 22;
			if(zms[i].x >= 100)
				zms[i].x -= zms[i].speed;
		}
	}
}