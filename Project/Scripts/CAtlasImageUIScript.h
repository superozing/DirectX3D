#pragma once
#include "CImageUIScript.h"
class CAtlasImageUIScript : public CImageUIScript
{
protected:
	Vec2 m_vLeftTop;
	Vec2 m_vSize;

public:
	void SetAtlasSize(Vec2 _lefttop, Vec2 _size)
	{
		m_vLeftTop = _lefttop;
		m_vSize	   = _size;
	}

public:
	void Apply();
	void init();

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CAtlasImageUIScript);
	CAtlasImageUIScript();
	CAtlasImageUIScript(const CAtlasImageUIScript& _Origin);
	CAtlasImageUIScript(UINT _type);
	~CAtlasImageUIScript();
};
