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
	void AddIdx();
	void SubIdx();

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
