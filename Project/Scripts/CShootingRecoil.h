#pragma once
#include <Engine/CScript.h>

// 사격 반동 클래스
// 키 매니저에게 오프셋을 주기 위해 수치를 조절해주는 래핑 클리스
class CShootingRecoil : public CScript
{
private:
	// 반동
	Vec2 m_vRecoil;

public:
	// 사격 반동 값을 설정하는 함수. 
	// 윈도우 좌표 기준임을 주의하세요
	void SetShootingRecoilValue(Vec2 _vShootingRecoil) { m_vRecoil = _vShootingRecoil; }

	// 사격 반동을 적용하는 함수(사격 시 호출)
	void ApplyShootingRecoil() const;

public:
	CLONE(CShootingRecoil);
	CShootingRecoil();
	~CShootingRecoil();
};
