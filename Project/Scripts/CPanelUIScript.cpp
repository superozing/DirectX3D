#include "pch.h"
#include "CPanelUIScript.h"
#include <Engine/CUIMgr.h>

CPanelUIScript::CPanelUIScript()
	: CUIScript((UINT)SCRIPT_TYPE::PANELUISCRIPT)
{
}

CPanelUIScript::CPanelUIScript(const CPanelUIScript& _Other)
	: CUIScript(_Other.GetScriptType())
{
}

CPanelUIScript::~CPanelUIScript()
{
}

void CPanelUIScript::tick()
{
	if (isFirstTick)
	{
		isFirstTick = false;
		MeshRender()->GetDynamicMaterial(0);
	}


	if (IsLBtnDown())
	{
		// 패널 드래그 앤 드롭

		// 패널의 움직임 정도 계산 - 차이를 구하는 것이기 때문에 굳이 world로 변환할 필요 없어보인다.
		Vec3 vDiff(m_vLbtnDownPos - CUIMgr::GetInst()->GetWorldMousePos(), 0.f);
		Vec3 vPos = Transform()->GetWorldPos();
		vPos -= vDiff;

		// 드래그 앤 드롭 사용 시, pos 변경 (여기 말고 바깥 if에 해도 될 듯)
		if (m_AllowDragAndDrop)
			Transform()->SetRelativePos(vPos);

		m_vLbtnDownPos = CUIMgr::GetInst()->GetWorldMousePos();
	}

	CUIScript::tick();


	if (m_AllowTexSet)
		MeshRender()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_PanelTex);
}

void CPanelUIScript::LBtnDown()
{
	// 마우스 왼쪽 버튼이 눌렸을 때 마우스의 위치를 기록
	m_vLbtnDownPos = CUIMgr::GetInst()->GetWorldMousePos();
}