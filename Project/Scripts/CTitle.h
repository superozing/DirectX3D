#pragma once

#include "CGameMode.h"

enum class TitleState
{
	RoROpen,
	RoRClose,
	BAOpen,
	BAClose,
	MenuSelect,
	END,
};

class CTitle : public CGameMode<CTitle>
{
private:
	float m_fAlphaTime;

	class CTitleTex* m_pRemnants;
	class CTitleTex* m_pBlueArchive;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CTitle);
	CTitle();
	~CTitle();

private:
	void RoROpenBegin();
	int	 RoROpenUpdate();
	void RoROpenEnd();

	void RoRCloseBegin();
	int	 RoRCloseUpdate();
	void RoRCloseEnd();

	void BAOpenBegin();
	int	 BAOpenUpdate();
	void BAOpenEnd();

	void BACloseBegin();
	int	 BACloseUpdate();
	void BACloseEnd();

	void MenuSelectBegin();
	int	 MenuSelectUpdate();
	void MenuSelectEnd();
};
