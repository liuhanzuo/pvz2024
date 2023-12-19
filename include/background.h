#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include "plant.h"
#include "sun.h"
#include "zombies.h"
#include "bullet.h"
#include "image.h"
#include <windows.h>
class BackGround{
    private:
    public:
        static const int MAX_PLANT_TYPE = 4, startx = 325 , dis = 65, starty = 2;
        int curx, cury, curzhiwu = 0;//0:ûѡ�У�k����k��
        int zmsamount = 25, zmscreate = 0, zmsdied = 0;
        static int status = 0;
        static const int width = 900;
        static const int height= 600;
        static const int rowl = 3;
        static const int columnl = 9;
        static const int ballmax = 10;
        static const int zmsmax = 50;
        static const int bulletmax = 50;
        static unsigned long long lastTime=0;
    public:
        Plant zw[rowl][columnl];
        Sun ball[ballmax];
        Zombies zms[zmsmax];
        Bullet bullets[bulletmax];
        IMAGE imgbg,imgbar;
        Atlas cards;
        int cursun=0;
        int time0=0;
        int suncount=0,fre=50;
    public:
        int GetDelay()const;
        bool fileexist(const char* name)const;
        void UpdateImage();
        void CollectSun(ExMessage* msg);
        void UserClick();
        void CreateSingleSun(int x,int y,int created);
        void MakeSuns();
        void SunFalls();
        void UpdateZombies();
};
#endif