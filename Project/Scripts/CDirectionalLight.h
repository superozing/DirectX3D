#pragma once
#include <Engine/CScript.h>
class CDirectionalLight : public CScript
{
private:
	CGameObject* m_Player;
	float		 m_fDistance;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout);
	virtual void LoadFromFile(ifstream& fin);

public:
	CLONE(CDirectionalLight);
	CDirectionalLight();
	~CDirectionalLight();
};
