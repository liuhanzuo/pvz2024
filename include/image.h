#ifndef __IMAGE_H__
#define __IMAGE_H__
#include "item.h"
#include <map>
#include <string>
class Atlas{
    public:
        IMAGE datas[BackGround::ATLAS_MAX];
        void Init();
        Atlas(){Init();}
};
#endif