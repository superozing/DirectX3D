#pragma once

#include <Engine\CScript.h>
#include <Engine\CTaskMgr.h>

class CLevelTransition : public CScript
{
private:
	tTask m_ChangeLevelTask;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CLevelTransition);
	CLevelTransition();
	~CLevelTransition();
};
