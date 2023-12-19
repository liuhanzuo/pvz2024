#ifndef __PLANT_H_
#define __PLANT_H_
#include "item.h"
class Plant:public Item{
    public:
    static const int PEA=1;
    static const int SUN=2;
    static const int MOUTH=3;
    public:
    Plant():type(0){}
    Plant(int tp):type(tp){}
    int type;//0 refers to no plants, and positive integeres refer to rank
	int eat;
	int hp;
    int timer;
    void GetImaged() override;
};
#endif