#pragma once
#include <Engine\CScript.h>

class CAfterImage : public CScript
{
private:
	AfterImageInfo m_info;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void UpdateData() override;
	virtual void Clear() override;

public:
	CLONE(CAfterImage);
	CAfterImage();
	~CAfterImage();
};
