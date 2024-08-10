#pragma once
#include <Engine/CScript.h>

class CCubePushScript : public CScript
{
	void resolveCollision(CGameObject* _Player);

public:
	virtual void begin() override;
	virtual void tick() override;
	virtual void Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider);
	virtual void SaveToFile(ofstream& fout);
	virtual void LoadFromFile(ifstream& fin);

public:
	CLONE(CCubePushScript);
	CCubePushScript();
	~CCubePushScript();
};
