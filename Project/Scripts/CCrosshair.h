#pragma once
#include <Engine\CScript.h>

class CImageUIScript;
class CPanelUIScript;

class CCrosshair : public CScript
{
private:
	// 조준선이 벌어지는 비율
	float m_fSpreadRatio;

	// 조준선 막대기 넓이
	float m_fBarThikness;

	// 조준선 막대기 길이
	float m_fLength;

	// 조준선 색상
	Vec4 m_CrosshairColor;

	// 사격 시스템 스크립트 - 정보를 얻어와 render에 사용
	// 현재 crosshair가 담당하는 사격에 관련된 모든 기능을 shootingsystem으로 옮겨주어야 함.
	// 옮겨볼까용?
	class CShootingSystemScript* m_pShootingSystem;

	CGameObject*		 m_pPlayer;
	class CPlayerScript* m_pPlayerScript;


public:
	virtual void begin() override;
	virtual void tick() override;

	//void SetSpreadRatio(float _fSpreadRatio) { m_fSpreadRatio = RoRMath::ClampFloat(_fSpreadRatio, 0.f, 1.f); }
	void Fire() { m_fSpreadRatio = RoRMath::ClampFloat(m_fSpreadRatio + 0.2f, 0.2f, 1.f); }
	void SetCrosshairColor(Vec4 _CrosshairColor) { m_CrosshairColor = _CrosshairColor; }

	//float GetSpreadRatio() const { return m_fSpreadRatio; }
	//Vec4  GetCrosshairColor() const { return m_CrosshairColor; }

	void SetShootingSystem(CShootingSystemScript* _pShootingSystem) { m_pShootingSystem = _pShootingSystem; }

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CCrosshair);
	CCrosshair();
	~CCrosshair();
};
