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
    public:
        void Init();
        bool Win()const;
        void CreateZombies();
        void UpdateStep();
        void Start();
        void Ready();
        void PlayVideo();
};
#endif 