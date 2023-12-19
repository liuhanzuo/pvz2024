#ifndef __ZOMBIES_H_
#define __ZOMBIES_H_
#include"putimage.h"
class Zombies:public Item{
	public:
	static const int WALK_IMAGE_INDEX=0;
	static const int DEAD_IMAGE_INDEX=1;
	static const int EAT_IMAGE_INDEX=2;
	static const int STAND_IMAGE_INDEX=3;
    public:
	int speed;
	int row;
	int hp;
	int died;
	int eat; 
	void GetImaged() override;
};
#endif