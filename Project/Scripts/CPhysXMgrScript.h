#pragma once
#include <Engine/CScript.h>

class CPhysXMgrScript : public CScript
{
private:
public:
public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override{};
	virtual void LoadFromFile(FILE* _File) override{};

public:
	CLONE_DISABLE(CPhysXMgrScript);
	CPhysXMgrScript();
	~CPhysXMgrScript();
};
