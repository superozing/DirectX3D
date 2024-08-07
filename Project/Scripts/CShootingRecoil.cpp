#include "pch.h"
#include "CShootingRecoil.h"
#include <Engine/CKeyMgr.h>

CShootingRecoil::CShootingRecoil()
{
}

CShootingRecoil::~CShootingRecoil()
{
}

void CShootingRecoil::ApplyShootingRecoil() const
{
	// 멤버로 설정된 값을 마우스 오프셋으로 넘겨줌.
	CKeyMgr::GetInst()->SetMouseOffset(m_vRecoil);
}
