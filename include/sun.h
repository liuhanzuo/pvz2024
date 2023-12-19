#ifndef __SUN_H_
#define __SUN_H_
#include"item.h"
class Sun:public Item{
    public:
	int desty;
	int time;
	int startx,starty;
	float xoff,yoff;
	int collect;
	int createdby;
	void GetImaged() override;
};
#endif