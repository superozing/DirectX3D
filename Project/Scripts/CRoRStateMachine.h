﻿#pragma once
class Coroutine;

template <class T> class CRoRStateMachine
{
	private:
	// 멤버는 int변수 밖에없지만
	// 직접 접근을 방지하기위한(setter강제) innerclass
	class StateManager
	{
		private:
		int m_PrevState;
		int m_CurState;
		CRoRStateMachine *m_Outer;

		private:
		StateManager() : m_PrevState(-1), m_CurState(-1), m_Outer(nullptr)
		{
		}

		public:
		StateManager(CRoRStateMachine *_Outer) : StateManager()
		{
			m_Outer = _Outer;
		};
		~StateManager(){};
		int GetPrevStateInt()
		{
			return m_CurState;
		}
		int GetCurStateInt()
		{
			return m_CurState;
		}
		void SetCurState(int _ToState)
		{
			T *m_pOwner = m_Outer->m_pOwner;

			if (!m_Outer->m_bLocked && m_CurState != _ToState)
			{
				m_Outer->m_bChangedStates = true;
				m_PrevState = m_CurState;
				m_CurState = _ToState;
				if (m_PrevState != -1 && m_Outer->m_vecEnds[m_PrevState] != nullptr)
				{
					(m_pOwner->*(m_Outer->m_vecEnds[m_PrevState]))();
				}
				if (m_Outer->m_vecBegins[m_CurState] != nullptr)
				{
					(m_pOwner->*(m_Outer->m_vecBegins[m_CurState]))();
				}
				if (m_Outer->m_vecCoroutines[m_CurState] != nullptr)
				{
					return;
				}
				m_Outer->currentCoroutine->Cancel();
			}
		}
		void ForceSetCurState(int _ToState);
		T *GetOwner()
		{
			return m_Outer->m_pOwner;
		}
	};
	StateManager m_StateMgr;
	T *m_pOwner;

	public:
	bool m_bChangedStates;
	bool m_bLog;
	bool m_bLocked;

	vector<void (T::*)()> m_vecBegins;
	vector<int (T::*)()> m_vecUpdates;
	vector<void (T::*)()> m_vecEnds;
	vector<void (T::*)()> m_vecCoroutines;
	vector<string> m_vecStateStrings;
	Coroutine *currentCoroutine;

	private:
	CRoRStateMachine() = delete;

	public:
	void SetCallbacks(int _State, string _StateName, int (T::*_Update)(), void (T::*_Begin)() = nullptr,
					  void (T::*_End)() = nullptr, void (T::*CoRoutine)() = nullptr)
	{
		m_vecStateStrings[_State] = _StateName;
		m_vecUpdates[_State] = _Update;
		m_vecBegins[_State] = _Begin;
		m_vecEnds[_State] = _End;
		m_vecCoroutines[_State] = CoRoutine;
	}
	void Update()
	{
		auto state = GetCurState();

		m_bChangedStates = false;
		if (m_vecUpdates[state] != NULL)
		{
			int _toState = (m_pOwner->*m_vecUpdates[state])();
			SetCurState(_toState);
		}
	}
	void Begin()
	{
		SetCurState(0);
	}
	void ForceState(int _ToState)
	{
		m_StateMgr.ForceSetCurState(_ToState);
	}
	void SetCurState(int _ToState)
	{
		m_StateMgr.SetCurState(_ToState);
	}
	int GetCurState()
	{
		return m_StateMgr.GetCurStateInt();
	}
	int GetPrevState()
	{
		return m_StateMgr.GetPrevStateInt();
	}

	// 로깅용
	void ReflectState(CGameObject *from, int index, string name);
	void LogState(int index);
	void LogAllStates();

	CRoRStateMachine(T *_pOwner, int maxStates = 1)
		: m_pOwner(_pOwner), m_StateMgr(StateManager(this)), m_bChangedStates(false), m_bLog(false), m_bLocked(false),
		  currentCoroutine(nullptr)
	{
		m_vecStateStrings.resize(maxStates);
		m_vecUpdates.resize(maxStates, nullptr);
		m_vecBegins.resize(maxStates, nullptr);
		m_vecEnds.resize(maxStates, nullptr);
		m_vecCoroutines.resize(maxStates, nullptr);
	}

	~CRoRStateMachine(){};
};

class Coroutine
{
	public:
	bool Active;
	void Update(){};
	Coroutine() : Active(false){};
	~Coroutine(){};
	void Cancel(){};
};