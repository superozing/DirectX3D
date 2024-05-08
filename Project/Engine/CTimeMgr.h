#pragma once

class CTimeMgr
	: public CManager<CTimeMgr>
{
	SINGLE(CTimeMgr);
private:
	// Larget Integer 는 그냥 8바이트 long long 타입 정수로 취급
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_PrevCount;
	LARGE_INTEGER	m_CurCount;

	double			m_DeltaTime;
	double			m_EngineDeltaTime;

	float			m_DTScale; // 델타 타임 배율

	bool			m_bLock; // DT 잠금
	
	UINT			m_iCall;
	double			m_Time;

	wchar_t			m_szText[256];

public:
	float GetDeltaTime() const { return (float)m_DeltaTime; }
	double GetDeltaTime_d() const { return m_DeltaTime; }

	float GetEngineDeltaTime() const { return (float)m_EngineDeltaTime; }
	double GetEngineDeltaTime_d() const { return m_EngineDeltaTime; }

	float GetDTScale() const { return m_DTScale; }
	void SetDTScale(float _DTScale) { m_DTScale = _DTScale; }

	void LockDeltaTime(bool _Lock) { m_bLock = _Lock; }

public:
	virtual void init() override;
	virtual void tick() override;
	virtual void enter() override {}
	virtual void exit() override;
	void render();
};

