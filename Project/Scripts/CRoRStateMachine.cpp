#include "pch.h"
#include "CRoRStateMachine.h"

template <class T> void CRoRStateMachine<T>::ReflectState(CGameObject *from, int index, string name)
{
}

template <class T> void CRoRStateMachine<T>::LogState(int index)
{
}

template <class T> void CRoRStateMachine<T>::LogAllStates()
{
}

template <class T> void CRoRStateMachine<T>::StateManager::ForceSetCurState(int _ToState)
{
	if (m_CurState != _ToState)
	{
		m_CurState = _ToState;
		return;
	}
	m_Outer->m_bChangedStates = true;
	m_PrevState = m_CurState;
	m_CurState = _ToState;
	if (m_PrevState != -1 && m_Outer->m_vecEnds[m_PrevState] != nullptr)
	{
		(m_Outer->m_vecEnds[m_PrevState])();
	}
	if (m_Outer->m_vecBegins[m_CurState] != nullptr)
	{
		m_Outer->m_vecBegins[m_CurState]();
	}
	if (m_Outer->m_vecCoroutines[m_CurState] != nullptr)
	{
		m_Outer->currentCoroutine->Replace(m_Outer->m_vecCoroutines[m_CurState]());
		return;
	}
	m_Outer->currentCoroutine->Cancel();
}