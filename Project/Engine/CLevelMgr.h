#pragma once
#include "singleton.h"

class CLevel;

class CLevelMgr :
    public CManager<CLevelMgr>
{
    SINGLE(CLevelMgr);
private:
    CLevel*         m_CurLevel;

    CGameObject*    m_EditorCam;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void enter() override;
    virtual void exit() override;

public:
    void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextStartState);
    void ChangeLevelState(LEVEL_STATE _State);

private:
    void ChangeLevel_Task(CLevel* _NextLevel, LEVEL_STATE _NextLevelState);


public:
    CLevel* GetCurrentLevel() { return m_CurLevel; }


    friend class CTaskMgr;
};

