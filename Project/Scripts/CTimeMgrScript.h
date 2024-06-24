#pragma once
#include <Engine/CScript.h>

class CTimeMgrScript : public CScript
{
private:
	float m_DTScale;
	int m_DTLock; // 원래라면 bool 형식을 사용해야 하지만, 현재 스크립트 파람에 bool이 없기에 int를 사용함.

public:
	void SetDTScale(float _DTScale);
	void LockDeltaTime(bool _DTLock);

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE_DISABLE(CTimeMgrScript);
	CTimeMgrScript();
	~CTimeMgrScript();
};
