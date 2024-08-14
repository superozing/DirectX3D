#pragma once
#include <Engine\CScript.h>

enum class ColorMode
{
	Original,
	Custom,
};

class CAfterImage : public CScript
{
private:
	AfterImageInfo m_info;
	float		   fUpdateTimer;
	float		   fSetLifeTime;
	string		   strDisplayColorMode;
	Vec4		   CustomColor;
	bool		   bDisplay;

	CStructuredBuffer* m_BoneArr[AfterImageMaxCount];

private:
	void UpdateBoneMatrix();
	void CalLifeTime(float _Time);
	void CalNodeRender();

public:
	void ChangeColorMode();
	void SetAlpha(float _Alpha);

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
	CAfterImage(const CAfterImage& _Origin);
	~CAfterImage();
};
