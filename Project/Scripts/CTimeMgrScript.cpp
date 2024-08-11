#include "pch.h"
#include "CTimeMgrScript.h"

#include <Engine/CTimeMgr.h>

#define TIMEMGR CTimeMgr::GetInst()

CTimeMgrScript::CTimeMgrScript()
	: CScript((UINT)SCRIPT_TYPE::TIMEMGRSCRIPT)
	, m_DTLock(0)
	, m_DTScale(1.f)
{
}

CTimeMgrScript::~CTimeMgrScript()
{
}

void CTimeMgrScript::begin()
{
	AppendScriptParam("DeltaTime Lock", SCRIPT_PARAM::INT, &(TIMEMGR->m_bLock));
	AppendScriptParam("DeltaTime Scale", SCRIPT_PARAM::FLOAT, &(TIMEMGR->m_DTScale), 0.f, 100.f, false, {}, true);
}

void CTimeMgrScript::tick()
{
	// 만약 ScriptParam에 함수를 호출하는 델리게이트 형식의 버튼이 있다면 매 틱마다 함수를 호출하지 않아도 될 것 같다.
	// LockDeltaTime(m_DTLock);
	// SetDTScale(m_DTScale);
}

void CTimeMgrScript::SetDTScale(float _DTScale)
{
	m_DTScale = _DTScale;
	TIMEMGR->SetDTScale(m_DTScale);
}

void CTimeMgrScript::LockDeltaTime(bool _DTLock)
{
	m_DTLock = _DTLock;
	TIMEMGR->LockDeltaTime(m_DTLock);
}

void CTimeMgrScript::SaveToFile(FILE* _File)
{
}

void CTimeMgrScript::LoadFromFile(FILE* _File)
{
}

#undef TIMEMGR