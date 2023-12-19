#ifndef __BULLET_H__
#define __BULLET_H__
class Bullet:public Item{
    public:
	int row;
	int speed;
	int blast;
	static const int BLAST_IMAGE_INDEX=1;
	static const int NORM_IMAGE_INDEX=0;
    void UpdateBullet();
	void GetImaged()override;
};
#endif