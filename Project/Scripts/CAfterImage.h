#pragma once
#include <Engine\CScript.h>

class CAfterImage : public CScript
{
private:
	AfterImageInfo m_info;
	float		   fUpdateTimer;
	float		   fSetLifeTime;
	bool		   bDisplay;

	CStructuredBuffer* m_BoneArr[AfterImageMaxCount];

private:
	void UpdateBoneMatrix();
	void CalLifeTime(float _Time);

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void ParticularUpdateData(string _Key) override;
	virtual void ParticularClear(string _Key) override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CAfterImage);
	CAfterImage();
	~CAfterImage();
};
