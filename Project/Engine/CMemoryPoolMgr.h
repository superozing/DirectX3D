#pragma once
#include "CManager.h"

class CMemoryPoolMgr : public CManager<CMemoryPoolMgr>
{
	SINGLE(CMemoryPoolMgr);

private:
	list<CGameObject*> m_listObjectPool;

	int iPoolMaxCount;
	int iCurPopCount;

public:
	void		 init();
	virtual void enter() override {}
	void		 tick();

public:
	CGameObject* PopObject();
	void		 PushObject(CGameObject* _Object);
};
