#pragma once
#include "CManager.h"

class CMemoryPoolMgr : public CManager<CMemoryPoolMgr>
{
	SINGLE(CMemoryPoolMgr);

private:
	vector<CGameObject*> m_vecObjectPool;

public:
	void		 init();
	virtual void enter() override {}
	void		 tick();

public:
	CGameObject* Allocate();
	void		 DeAllocate(CGameObject* _Object);
};
