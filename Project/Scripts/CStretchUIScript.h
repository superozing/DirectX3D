#pragma once
#include "CAtlasImageUIScript.h"
class CStretchUIScript : public CAtlasImageUIScript
{
private:
	Vec2 m_vBaseSize;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CStretchUIScript);
	CStretchUIScript();
	CStretchUIScript(const CStretchUIScript& _Origin);
	~CStretchUIScript();
};
