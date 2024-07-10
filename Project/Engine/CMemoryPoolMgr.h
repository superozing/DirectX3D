#pragma once

#include "CManager.h"

class CMemoryPool;

class CMemoryPoolMgr : public CManager<CMemoryPoolMgr>
{
	SINGLE(CMemoryPoolMgr);

private:
	CMemoryPool* m_pPool;

	CGameObject* m_pMemoryPoolEX;

public:
	CGameObject* PopObject();
	void		 PushObject(CGameObject* _Object);

	int GetCurrentCount();

	void		 SetEX(CGameObject* pObj) { m_pMemoryPoolEX = pObj; }
	CGameObject* GetEX() { return m_pMemoryPoolEX; }

public:
	virtual void tick() override;
	virtual void init() override{};
	void		 begin(wstring strPrefabRelativePath);
	virtual void enter() override{};
};
