﻿#pragma once

#include <Engine\CScript.h>

class CMemoryPoolMgr : public CScript
{
private:
	list<CGameObject*> m_listObjectPool;

	int iPoolMaxCount;
	int iCurPopCount;

public:
	virtual void tick() override;
	virtual void begin() override;

	virtual void SaveToFile(FILE* _File) override{};
	virtual void LoadFromFile(FILE* _File) override{};

public:
	CLONE(CMemoryPoolMgr);
	CMemoryPoolMgr();
	~CMemoryPoolMgr();
};
