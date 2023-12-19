#ifndef __ZOMBIES_H_
#define __ZOMBIES_H_
#include"putimage.h"
class Zombies:public Item{
    public:
	int speed;
	int row;
	int hp;
	int died;
	int eat; 
};
#endif