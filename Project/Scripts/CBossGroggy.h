#pragma once
#include "CProgressBar.h"

class CBossGroggy : public CScript
{
private:
	// Groggy Gauge UI
	CImageUIScript* m_pGroggyGaugeUI;

	// 그로기 수치를 받아오기 위해 가리켜야 해요.
	class CBossScript* m_pBossScript;

public:
	virtual void begin() override;
	virtual void tick() override;

	void SetBossScript(CBossScript* _pBossScript) { m_pBossScript = _pBossScript; }

public:
	CLONE(CBossGroggy);
	CBossGroggy();
	~CBossGroggy();
};
