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

public:
	CLONE(CMemoryPoolMgrScript);
	CMemoryPoolMgrScript();
	~CMemoryPoolMgrScript();
};
