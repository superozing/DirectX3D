#pragma once

#include <Engine/CScript.h>
class CTitle : public CScript
{
private:
	bool m_bRemnantsEnd;
	bool m_bRemnantsDarkStart;
	bool m_bBlueArchiveEnd;
	bool m_bBlueArchiveBrightStart;
	bool m_bBlueArchiveDarkStart;

	bool			 m_bTitleShow;
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
	void DrawRemnants();
	void RemnantsBrightStart();
	void RemnantsDarkStart();

	void DrawBlueArchive();
	void BlueArchiveBrightStart();
	void BlueArchiveDarkStart();

	void SwitchToNextTexture();
};
