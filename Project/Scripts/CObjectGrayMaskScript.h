#pragma once
#include "Engine\CScript.h"

class CObjectGrayMaskScript : public CScript
{
public:
	virtual void begin() override;
	virtual void tick() override;
	virtual void UpdateData() override;
	virtual void Clear() override;
	virtual void SaveToFile(ofstream& fout);
	virtual void LoadFromFile(ifstream& fin);

public:
	CLONE(CObjectGrayMaskScript);
	CObjectGrayMaskScript();
	~CObjectGrayMaskScript();
};
