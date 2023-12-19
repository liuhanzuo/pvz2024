#include "image.h"
void Atlas::PrintImage(const Item &it){
    if(it.online){
        it.time++;
    }
}

void Atlas::Init()
{
    memset(datas,0,sizeof(datas));
}
