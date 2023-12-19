#ifndef __PLANT_H_
#define __PLANT_H_

class Plant:public Item{
    public:
    int type=0;//0 refers to no plants, and positive integeres refer to rank
	int eat;
	int hp;
    int timer;
};
#endif