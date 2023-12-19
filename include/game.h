#ifndef __GAME_H_
#define __GAME_H_
#include "background.h"
#include <graphics.h>
class Game{
    public:
        BackGround back;
    public:
        static const int MAX_PLANT_NUM=10;
        static const int status=0;
        int needsun[MAX_PLANT_NUM];
        IMAGE imgbg,imgbar,imgcards[16],* imgzhiwu[16][32];
        auto& zw=back.zw;
        auto& ball=back.ball;
        auto& zms=back.zms;
        auto& bullets=back.bullets;
    public:
        
    public:
        IMAGE balls[30];
        IMAGE z[25];
        IMAGE zmdied[20];
        IMAGE zmseat[21];
        IMAGE zmsstand[15];
        IMAGE bulletn,bulletb;
        IMAGE imgballblast[4];
    public:
        void Init();
        bool Win()const;
        void CreateZombies();
        void UpdateStep();
        void Start();
        void Ready();
        void PlayVideo();
        Game();
};
#endif 