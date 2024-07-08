#pragma once
#include "CManager.h"

class CMemoryPool : public CManager<CMemoryPool>
{
	SINGLE(CMemoryPool);

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

	list<CGameObject*> GetList() { return m_listObjectPool; }
};
