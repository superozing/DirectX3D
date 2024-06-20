#include "pch.h"
#include "CUIScript.h"

#include "CDevice.h"
#include "CCamera.h"
#include "CUIMgr.h"

CUIScript::CUIScript()
	: CScript(NULL)
	, m_ParentUI(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{
}

CUIScript::CUIScript(UINT _ScriptType)
	: CScript(_ScriptType)
	, m_ParentUI(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{
}

CUIScript::CUIScript(const CUIScript& _Other)
	: CScript(_Other.GetScriptType())
	, m_ParentUI(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{
	for (size_t i = 0; i < _Other.m_vecChildUI.size(); ++i)
	{
		AddChildUI(_Other.m_vecChildUI[i]->Clone());
	}
}

CUIScript::~CUIScript()
{
}

void CUIScript::tick()
{
	// 최종 좌표 연산하기
	m_vFinalPos = Transform()->GetWorldPos();

	// UIMgr tick()에서 계산된 월드 마우스 좌표를 가지고 오기
	Vec2 vWorldMousePos = CUIMgr::GetInst()->GetWorldMousePos();

	// 이전 마우스 상태 저장
	m_bMouseOn_Prev = m_bMouseOn;

	// WorldScale 사용해서 마우스 위치와 겹치는지 체크
	Vec3 vWorldScale = Transform()->GetWorldScale();

	Vec2 vLT = Vec2(m_vFinalPos.x - vWorldScale.x / 2, m_vFinalPos.y - vWorldScale.y / 2);
	Vec2 vRB = Vec2(m_vFinalPos.x + vWorldScale.x / 2, m_vFinalPos.y + vWorldScale.y / 2);

	if (vLT.x < vWorldMousePos.x && vWorldMousePos.x < vRB.x &&
		vLT.y < vWorldMousePos.y && vWorldMousePos.y < vRB.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}

	// 자식 UI 들 Tick 호출
	//for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	//{
	//	m_vecChildUI[i]->tick(_DT);
	//}
}