#pragma once
#include "singleton.h"

template <typename T> class CManager : public CSingleton<T>
{
public:
	virtual void init() {}
	virtual void tick() {}
	virtual void enter() = 0;
	virtual void exit() {}
};
