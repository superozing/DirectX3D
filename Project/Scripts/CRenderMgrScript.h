#pragma once
#include <Engine/CScript.h>

typedef float(*LerpFloatFuncPtr)(float, float, float);

struct TestPram
{
	float T1 = 0.f;
	float T2 = 0.f;
	float T3 = 0.f;
	float result = 0.f;
};

class CRenderMgrScript :
    public CScript
{
private:

public:
	static TestPram TestParam;
	TestPram m_TestParam;
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

	static void LerpTest()
	{
		TestParam.result = RoRMath::Lerp(TestParam.T1, TestParam.T2, TestParam.T3);
	}

	void m_LerpTest()
	{
		m_TestParam.result = RoRMath::Lerp(m_TestParam.T1, m_TestParam.T2, m_TestParam.T3);
	}

public:
	CLONE_DISABLE(CRenderMgrScript);
	CRenderMgrScript();
	~CRenderMgrScript();
};
