﻿#pragma once
#include <Engine/CManager.h>

struct tRes
{
	Vec2 res;
	bool window;
};
class CEnvMgr : public CManager<CEnvMgr>
{
	SINGLE(CEnvMgr);

private:
	tRes		 m_tRes;
	string		 m_strLevelRelativePath;
	vector<bool> m_bImguiActivate;
	float		 m_fMasterVolume;

public:
	tRes   GetResolutionData() { return m_tRes; }
	string GetLevelRelativePath() { return m_strLevelRelativePath; }

public:
	void init();
	void ImguiInit();
	void exit();
	void tick();

	virtual void enter() {}
};
