#ifndef __PEASHOOTER_H__
#define __PEASHOOTER_H__
#include "plant.h"
class PeaShooter:public Plant{
    public:
    PeaShooter():Plant(PEA){}
    void Shoot();
};
#endif