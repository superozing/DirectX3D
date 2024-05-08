#pragma once
#include "singleton.h"

template<typename T>
class CManager :
    public CSingleton<T>
{
public:
    virtual void init() {}
    virtual void enter() {}
    virtual void tick() {}
};

