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
        static const int width = 900;
        static const int height= 600;
        static const int rowl = 3;
        static const int columnl = 9;
        static const int ballmax = 10;
        static const int zmsmax = 50;
        static const int bulletmax = 50;
    public:
        Plant zw[rowl][columnl];
        Sun ball[ballmax];
        Zombies zms[zmsmax];
        Bullet bullets[bulletmax];
        Image im;
        int cursun;
    public:
        int getdelay()const;
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