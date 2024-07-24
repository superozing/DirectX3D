#pragma once
#include "singleton.h"

// 플레이 상태가 아니면 리턴합니다.
#define RETURN_IF_NOT_PLAYING                                                     \
	if (LEVEL_STATE::PLAY != CLevelMgr::GetInst()->GetCurrentLevel()->GetState()) \
		return;

typedef CLevel* (*Restart_Level)(const string& _strLevelPath);
typedef void	(*Save_CheckPoint)(CLevel* _Level);

enum class Client_Function_Type
{
	CIMGUIMGR_ENTER,
};

class CLevel;

class CLevelMgr : public CManager<CLevelMgr>
{
	SINGLE(CLevelMgr);

private:
	CLevel* m_CurLevel;
	string	m_strPrevLevelPath;

public:
	static Restart_Level   LevelChangeFunc;
	static Save_CheckPoint CheckPointFunc;

public:
	virtual void init() override;
	virtual void tick() override;
	virtual void enter() override;
	virtual void exit() override;

public:
	void ChangeLevelState(LEVEL_STATE _State);

private:
	void ChangeLevel_Task(CLevel* _NextLevel, LEVEL_STATE _NextLevelState);

public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }
	void	RegisterClientFunction(Client_Function_Type _Type, std::function<void()> _MemberFunction);

	string GetstrPrevLevelPath() { return m_strPrevLevelPath; }
	void   SetstrPrevLevelPath(string CurLevel) { m_strPrevLevelPath = CurLevel; }

private:
	unordered_map<Client_Function_Type, std::function<void()>> m_MapClientFunc;

	friend class CTaskMgr;
};
