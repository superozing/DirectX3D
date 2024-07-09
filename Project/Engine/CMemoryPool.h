#pragma once
#include "CManager.h"

class CMemoryPool : public CManager<CMemoryPool>
{
	SINGLE(CMemoryPool);

private:
	list<CGameObject*> m_listObjectPool;

	int iPoolMaxCount;
	int iCurPopCount;

	CGameObject* PoolMgr;

public:
	void		 init();
	virtual void enter() override {}
	void		 tick();

public:
	CGameObject* PopObject();
	void		 PushObject(CGameObject* _Object);

	void		 RegisterPoolMgr(CGameObject* _Mgr) { PoolMgr = _Mgr; }
	CGameObject* GetPoolMgr() { return PoolMgr; }

	list<CGameObject*> GetList() { return m_listObjectPool; }
};
