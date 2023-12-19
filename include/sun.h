#ifndef __SUN_H_
#define __SUN_H_
#include"putimage.h"
class Sun{
    public:
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
};
#endif