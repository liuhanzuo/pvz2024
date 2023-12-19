#ifndef __ITEM_H__
#define __ITEM_H__
#include "image.h"
#include "background.h"
class Item{
    public:
    int x,y;
    int frameindex;
    bool online;
    BackGround* back;
    static Atlas im[10];
    virtual void GetImaged();
};
#endif