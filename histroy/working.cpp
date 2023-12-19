#include<cstdio>
#include<graphics.h>
#include<time.h>
#include<mmsystem.h>
#include<Windows.h>
//Q�ǲ��ӣ��ո��ʹ��ʬ�����ٶȱ�Ϊ�屶��ͬʱ�㶹����һ���ߵ��㶹������Ϊ����
#pragma comment(lib,"winmm.lib")
const int width = 900;
const int height= 600;
const int rowl = 3;
const int columnl = 9;
const int ballmax = 10;

int cnt = 4, startx = 325  , dis = 65, starty = 2;
int curx, cury, curzhiwu = 0;//0:ûѡ�У�k����k��
int zmsamount = 25, zmscreate = 0, zmsdied = 0;
static int status = 0;

struct plant {
	int type=0;//0 refers to no plants, and positive integeres refer to rank
	int frameindex=0;//֡�����
	int eat;
	int hp;
    int timer;
}zw[rowl][columnl];
int needsun[10];
struct sunshine {
	int x, y;
	int frameindex;
	int desty;//Ŀ��λ��y����
	int flag;//�Ƿ�����ʾ
	int time;
	int startx;
	int starty;
	float xoff;
	float yoff;
	int collect;
	int createdby;
}ball[ballmax];
IMAGE balls[30];
int cursun = 150;
//����أ������п��ܵ�·�����ڳ����ÿ����һ��������
struct zm {
	int x;
	int y;
	int frameindex;
	int speed;
	int flag;
	int row;
	int hp;
	int died;
	int eat;
}zms[50];
IMAGE z[25];
IMAGE zmdied[20];
IMAGE zmseat[21];
IMAGE zmsstand[15];
struct bullet { 
	int x;
	int y;
	int row;
	int speed;
	int flag;
	int blast;//�Ƿ�����ը
	int frameindex;//֡���
}bullets[50];
IMAGE bulletn,bulletb;
IMAGE imgballblast[4];

//PIG
void _putimagePNG(int  picture_x, int picture_y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ��ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ��ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
					| (sb * sa / 255 + db * (255 - sa) / 255);
			}
		}
	}
}
void putimagePNG(int x, int y, IMAGE* picture) {

	IMAGE imgTmp, imgTmp2, imgTmp3;
	int winWidth = getwidth();
	int winHeight = getheight();
	if (y < 0) {
		SetWorkingImage(picture);
		getimage(&imgTmp, 0, -y,
			picture->getwidth(), picture->getheight() + y);
		SetWorkingImage();
		y = 0;
		picture = &imgTmp;
	}
	else if (y >= getheight() || x >= getwidth()) {
		return;
	}
	else if (y + picture->getheight() > winHeight) {
		SetWorkingImage(picture);
		getimage(&imgTmp, x, y, picture->getwidth(), winHeight - y);
		SetWorkingImage();
		picture = &imgTmp;
	}

	if (x < 0) {
		SetWorkingImage(picture);
		getimage(&imgTmp2, -x, 0, picture->getwidth() + x, picture->getheight());
		SetWorkingImage();
		x = 0;
		picture = &imgTmp2;
	}

	if (x > winWidth - picture->getwidth()) {
		SetWorkingImage(picture);
		getimage(&imgTmp3, 0, 0, winWidth - x, picture->getheight());
		SetWorkingImage();
		picture = &imgTmp3;
	}


	_putimagePNG(x, y, picture);
}
//PIGEND

int getdelay() {
	static unsigned long long lastTime = 0;
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
bool fileexist(const char* name) {
	FILE* fp = fopen(name, "r");
	if (fp) {
		fclose(fp);
	}
	return fp != NULL;
}
using namespace std;

IMAGE imgbg,imgbar,imgcards[16],* imgzhiwu[16][32];

void gameInit() {
	//������Ϸ����ͼƬ
	//���ַ����ĳ� ���ֽ��ַ���
	loadimage(&imgbg, "images/bg.jpg");
	loadimage(&imgbar, "images/bar4.png");
	memset(imgzhiwu, 0, sizeof(imgzhiwu));
	memset(zw, 0, sizeof(zw));
	//������Ϸ����
	//��ʼ��ֲ�￨��
	char name[64];
	for (int i = 0; i < cnt; i++) {
		//����ֲ�￨�Ƶ��ļ���
		sprintf_s(name, sizeof(name), "images/Cards/card_%d.png", i + 1);
		loadimage(&imgcards[i], name);
		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "images/zhiwu/%d/%d.png", i, j + 1);
			//���ж��ļ�ʱ�Ƿ����
			if (fileexist(name)) {
				//Ŀǰָ��Ϊ�գ���Ҫ�ȷ����ڴ�
				imgzhiwu[i][j] = new IMAGE;
				loadimage(imgzhiwu[i][j], name);
			}
			else {
				break;
			}
		}
	}
	needsun[1] = 100;
	needsun[2] = 50;
	needsun[3] = 150;
	needsun[4] = 50;
	memset(ball, 0, sizeof(ball));
	for (int i = 1; i <= 29; i++) {
		sprintf_s(name, sizeof(name), "images/sunshine/%d.png", i);
		loadimage(&balls[i-1], name);
	}
	memset(zms, 0, sizeof(zms));
	for (int i = 1; i <= 22; i++) {
		sprintf_s(name, sizeof(name), "images/zm/%d.png", i);
		loadimage(&z[i-1], name);
	}
	memset(bullets, 0, sizeof(bullets));
	loadimage(&bulletn, "images/bullets/bullet_normal.png");
	//��ʼ��֡����
	loadimage(&imgballblast[3], "images/bullets/bullet_blast.png");
	for (int i = 0; i < 3; i++) {
		float k = (i + 1) * 0; 2;
		loadimage(&imgballblast[i], "images/bulltes/bullet_blast.png", imgballblast[3].getwidth() * k, imgballblast[3].getheight()*k);
	}
	for (int i = 0; i < 20; i++) {
		sprintf_s(name,sizeof(name), "images/zm_dead/%d.png", i + 1);
		loadimage(&zmdied[i], name);
	}
	for (int i = 0; i < 21; i++) {
		sprintf_s(name, sizeof(name), "images/zm_eat/%d.png", i + 1);
		loadimage(&zmseat[i], name);
	}
	for (int i = 0; i < 11; i++) {
		sprintf_s(name, sizeof(name), "images/zm_stand/%d.png", i + 1);
		loadimage(&zmsstand[i], name);
	}
	//���
	srand(time(NULL));
	//���ɽ���
	initgraph(width, height, 1);
	//��������
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	strcpy(f.lfFaceName, "Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY;//�����Ч��
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setcolor(BLACK);
}

void imagesunshine() {
	//��ʾ����
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

void updateimage(){
	//˫����
	BeginBatchDraw();//��ʼ����
	putimage(-112, 0, &imgbg);
	putimage(250, 0, &imgbar);
	for (int i = 0; i < cnt; i++) {
		int x = startx + i * dis;
		int y = starty;
		putimage(x, y, &imgcards[i]);
	}
	for (int i = 0; i < rowl; i++) {
		for (int j = 0; j < columnl; j++) {
			int x = 256 -112 + j * 81;
			int y = 179 + i * 102;
			int tp = zw[i][j].type;
			IMAGE* img = imgzhiwu[zw[i][j].type - 1][zw[i][j].frameindex];
			if (tp&&tp!=3) {
				putimagePNG(x, y, img);
			}
			else if (tp == 3) {
				putimagePNG(x, y - 25, img);
			}
		}
	}
	//��Ⱦ�϶��е�ֲ��
	if (curzhiwu) {
		IMAGE* img = imgzhiwu[curzhiwu - 1][0];
		putimagePNG(curx - img->getwidth() / 2, cury - img->getheight() / 2, img);
	}
	imagesunshine();
	imagezombies();
	imagebullet();
	char text[16];
	sprintf_s(text, sizeof(text), "%d", cursun);
	outtextxy(276,55,text);
	EndBatchDraw();
}

bool win() {
	if (zmsdied >= zmsamount) {
		IMAGE imgwon;
		printf("win\n");
		loadimage(&imgwon, "images/Credits_ZombieNote.png");
		putimagePNG(150, 75, &imgwon);
		mciSendString("play images/win.mp3", 0, 0, 0);
		return false;
	}
	return true;
}

void collectsunshine(ExMessage* msg) {
	int w = balls[0].getwidth();
	int h = balls[0].getheight();
	for (int i = 0; i < ballmax; i++) {
		if (ball[i].flag && !ball[i].collect && (ball[i].x - msg->x + w / 2) * (ball[i].x - msg->x + w / 2)
			+ (ball[i].y - msg->y+ h/2) * (ball[i].y - msg->y+ h/2) <= (w*w+h*h)/4) {
			ball[i].collect = 1;
			ball[i].startx = ball[i].x;
			ball[i].starty = ball[i].y;
		}
	}
}

void userclick() {
	//�����Ϣ���ƶ�֮��ģ�
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
			collectsunshine(&msg);
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

void createsun(int x, int y, int created) {
	//���������ȡһ�����õ�
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
	//ÿ��fre�Ͳ���һ������
	static int count = 0;
	static int fre = 50;
	count++;
	if (count < fre) {
		return;
	}
	count = 0;
	fre = 50 + rand() % 50;
	int x = 260 -112 + rand() % (850 - 260);
	int y = 80;
	createsun(x ,y, 0);
}

void sunflowercreatesunshine() {
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
				//���ﻹ��Ҫ�Ż������ԭʼλ�õ��ռ���
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

void createzombies() {
	if (zmscreate > zmsamount) {
		return;
	}
	static int count = 0;
	static int fr=150;
	static int fre = 150;
	if (GetAsyncKeyState(VK_SPACE)) {
		fr = 30;
	}
	count++;
	if (count < fre) {
		return;
	}
	count = 0;
	fre = fr + rand() % (fr/3);
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

void shoot() {
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

void updatebullet() {
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

//PIG
void collisionbullet2zm() {
	int bulletmax = 50;
	int zmsmax = 50;
	for (int i = 0; i < bulletmax; i++) {
		if (!bullets[i].flag || bullets[i].blast) {
			continue;
		}
		for (int k = 0; k < zmsmax; k++) {
			if (!zms[k].flag||zms[k].died) {
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
			int zwx = 256 -112 + k * 81;
			int x1 = zwx + 10;
			int x2 = zwx + 60;
			int x3 = zms[i].x + 80;
			if (x3 > x1 && x3 < x2) {
				if (zms[i].eat) {
					zw[row][k].hp-=2;
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
//PIGEND

void updategame() {
	for (int i = 0; i < rowl; i++) {
		for (int j = 0; j < columnl; j++) {
			if (!zw[i][j].type) {
				continue;
			}
			zw[i][j].frameindex++;
			if (imgzhiwu[zw[i][j].type - 1][zw[i][j].frameindex] == NULL)
				zw[i][j].frameindex = 0;
		}
	}
	createsunshine();
	updatesunshine();
	createzombies();
	updatezombies();
	shoot();
	updatebullet();
	collisioncheck();
}

void start() {
	//���������˵�
	IMAGE imgbg0,imgbg1,imgbg2;
	loadimage(&imgbg0, "images/menu.png");
	loadimage(&imgbg1, "images/menu1.png");
	loadimage(&imgbg2, "images/menu2.png");
	int flag0 = 0;
	while (true) {
		BeginBatchDraw();
		putimage(0, 0, &imgbg0);
		putimagePNG(474, 75, flag0 ? &imgbg2 : &imgbg1);
		EndBatchDraw();
		ExMessage msg;
		if (!peekmessage(&msg)) {
			continue;
		}
		if (msg.x > 474 && msg.x < (474 + 300) 
			&& msg.y > 75 && msg.y < (75 + 140)) {
			flag0 = 1;
		}
		else {
			flag0 = 0;
		}
		if (msg.message == WM_LBUTTONUP && flag0) {
			EndBatchDraw();
			break;
		}
	}
}

void ready() {
	IMAGE r[4];
	loadimage(&imgbg, "images/bg.jpg");
	loadimage(&r[1], "images/StartReady.png");
	loadimage(&r[2], "images/StartSet.png");
	loadimage(&r[3], "images/StartPlant.png");
	putimage(-112, 0, &imgbg);
	int qwq = 0;
	for (int i = 1; i <= 3; i++) {
		while (qwq <= 600) {
			qwq += getdelay();
			putimage(400, 200, &r[i]);
		}
		qwq = 0;
	}
}

void view() {
	int xmin = width - imgbg.getwidth();
	int xs[9] = { 550,530,630,530,515,565,605,705,690 };
	int ys[9] = { 80,160,170,200,270,370,340,280,340 };
	int index[9];
	for (int i = 0; i < 9; i++) {
		index[i] = rand() % 11;
	}
	int count = 0;
	for (int x = 0; x >= xmin; x -= 3) {
		BeginBatchDraw();
		putimage(x, 0, &imgbg);
		count++;
		for (int k = 0; k < 9; k++) {
			putimagePNG(xs[k]-xmin+x,ys[k],&zmsstand[index[k]]);
			if (count >= 10) {
				index[k] = (index[k] + 1) % 11;
			}
		}
		if (count >= 10) {
			count = 0;
		}
		EndBatchDraw();
		Sleep(5);
	}
	//ͣ��1s����
	for (int i = 0; i < 100; i++) {
		BeginBatchDraw();
		putimage(xmin, 0, &imgbg);
		for (int k = 0; k < 9; k++) {
			putimagePNG(xs[k], ys[k], &zmsstand[index[k]]);
			index[k] = (index[k] + 1) % 11;
		}
		EndBatchDraw();
		Sleep(30);
	}
	for (int x = xmin; x <= -112; x += 3) {
		BeginBatchDraw();
		putimage(x, 0, &imgbg);
		count++;
		for (int k = 0; k < 9; k++) {
			putimagePNG(xs[k] - xmin + x, ys[k], &zmsstand[index[k]]);
			if (count >= 10) {
				index[k] = (index[k] + 1) % 11;
			}
		}
		if (count >= 10) {
			count = 0;
		}
		EndBatchDraw();
		Sleep(5);
	}
}
int main() {
	gameInit();
	int time = 0;
	bool flag = true;
	//�����˵�
	start();
	//ת��
	view();
	//��������
	ready();
	while (true) {
		userclick();
		time += getdelay();
		if (time > 50) {
			flag = true;
			time = 0;
		}
		if (flag) {
			flag = false;
			updateimage();
			updategame();
		}
		if (!win()) {
			break;
		}
	}	
	system("pause");
	return 0;
}
