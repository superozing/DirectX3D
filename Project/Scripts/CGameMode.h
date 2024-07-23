#pragma once
#include <Engine\CScript.h>

#include "CRoRStateMachine.h"

template <typename T> class CGameMode : public CScript
{
protected:
	CRoRStateMachine<T>* m_FSM;

public:
	CLONE(CGameMode);
	CGameMode(UINT _scriptType);
	~CGameMode();
};

template <typename T>
CGameMode<T>::CGameMode(UINT _scriptType)
	: CScript(_scriptType)
{
}

template <typename T> CGameMode<T>::~CGameMode()
{
}
