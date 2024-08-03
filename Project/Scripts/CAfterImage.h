#pragma once
#include <Engine\CScript.h>

class CAfterImage : public CScript
{
private:
	AfterImageInfo m_info;
	float		   fUpdateTimer;
	bool		   bDisplayNode;

	vector<Matrix> vec_ParentsBones;

	CStructuredBuffer* m_arr[AfterImageMaxCount];

private:
	void UpdateBoneMatrix();

public:
	vector<Matrix> GetAfterImageBoneMatrix() { return vec_ParentsBones; }

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
