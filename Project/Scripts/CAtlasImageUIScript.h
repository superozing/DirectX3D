#pragma once
#include "CImageUIScript.h"
class CAtlasImageUIScript : public CImageUIScript
{
private:
	Vec2 m_vLeftTop;
	Vec2 m_vSize;

public:
	void Apply();

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CAtlasImageUIScript);
	CAtlasImageUIScript();
	CAtlasImageUIScript(const CAtlasImageUIScript& _Origin);
	~CAtlasImageUIScript();
};
