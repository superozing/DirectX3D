﻿#pragma once
#include <Engine/CScript.h>

class CBloomScript : public CScript
{
private:
	tBloom			   m_Info		  = {};
	bool			   m_bBloomActive = true;
	CStructuredBuffer* m_Buffer		  = nullptr;

public:
	virtual void begin() override;
	virtual void tick() override;
	virtual void UpdateData() override;
	virtual void Clear() override;
	virtual void SaveToFile(ofstream& fout);
	virtual void LoadFromFile(ifstream& fin);

public:
	CLONE(CBloomScript);
	CBloomScript();
	CBloomScript(const CBloomScript&);
	~CBloomScript();
};
