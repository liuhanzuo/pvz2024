#ifndef __BULLET_H__
#define __BULLET_H__
class Bullet:public Item{
    public:
	int row;
	int speed;
	int blast;
    void UpdateBullet();
};
#endif