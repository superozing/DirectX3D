#pragma once
#include "UI.h"

class LogUI : public UI
{
private:
	vector<tLog> m_vectLog;
	ImVec4		 m_LogColor[(UINT)Log_Level::END];

	ImVec2 m_AvailableSize;

	int				m_iLoglvMask;
	ImGuiTextFilter m_TextFilter;

	bool m_LoglvCheckBox[4];
	bool m_bLogTimePrint;
	char m_LogTextFilter[255];

	float m_fLastScrollY;

public:
	ImVec4 GetLogColor(tLog Log);

	void CheckLevelMask();

	bool CheckSearchDisplay(string msg);
	bool CheckLogLvDisplay(Log_Level Loglv);

	bool* GetLogLvFilter() { return m_LoglvCheckBox; }
	char* GetLogTextFilter() { return m_LogTextFilter; }

public:
	virtual void tick() override;
	virtual void render_update() override;

public:
	LogUI();
	~LogUI();
};
