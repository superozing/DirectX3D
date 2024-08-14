#pragma once
#include <Engine\CScript.h>

enum class TitleButton
{
	Play,
	Settings,
	Exit,
	END
};
class CButtons : public CScript
{
private:
	int							m_iIdx;
	vector<class CBtnUIScript*> m_vecButtons;

public:
	void		AddIdx();
	void		SubIdx();
	bool		m_started		= false;
	bool		m_LevelChanged	= false;
	float		m_acctime		= 0.f;
	const float m_ThresholdTime = 3.f;

	static void (*Setting)();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CButtons);
	CButtons();
	~CButtons();

private:
	void Play();
	void Settings();
	void Exit();
};
