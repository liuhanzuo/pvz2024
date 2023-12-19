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
        Atlas* im[BackGround::ATLAS_MAX];
        virtual void GetImaged();
        void Init();
        Item(Atlas ** ptr){
            for(int i=0;i<BackGround::ATLAS_MAX;i++){
                im[i]=ptr[i];
            }
        }
};
#endif