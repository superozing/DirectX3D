#pragma once

#include <Engine\CScript.h>

class CMemoryPoolMgrScript : public CScript
{
private:
public:
	virtual void tick() override;
	virtual void begin() override;

	virtual void SaveToFile(FILE* _File) override{};
	virtual void LoadFromFile(FILE* _File) override{};

public:
	CGameObject* PopObject(string _strMapKey);
	void		 PushObject(CGameObject* _Object);
	void		 PushObject(string _strMapKey, CGameObject* _Object);

	void PoolSet(string _strMapKey, int _Count);

public:
	CLONE(CMemoryPoolMgrScript);
	CMemoryPoolMgrScript();
	~CMemoryPoolMgrScript();
};
