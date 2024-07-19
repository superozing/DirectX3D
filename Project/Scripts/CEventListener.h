﻿#pragma once
#include <Engine\CScript.h>

class CEventListener : public CScript
{
private:
	/// @brief 판정하고싶은 스크립트들을 갖고있는 벡터입니다.
	vector<SCRIPT_TYPE> m_vecTargets;

	/// @brief 판정하는 바운더리 안에 있는 해당 스크립트를 갖고있는 오브젝트 개수입니다.
	int m_iInternalTargetCnt;

	/// @brief 디버깅을 위한 변수입니다.
	bool m_bDrawing = true;

public:
	/// @brief 판정하고 싶은 스크립트를 추가합니다.
	void AddTarget(SCRIPT_TYPE _type);
	/// @brief 판정하고 싶은 스크립트를 제거합니다.
	void SubTarget(SCRIPT_TYPE _type);
	/// @brief 판정하고 싶은 스크립트 보유 목록을 가져옵니다.
	const vector<SCRIPT_TYPE>& GetTargets() { return m_vecTargets; }

	bool HasTargets() { return m_iInternalTargetCnt; }

public:
	virtual void tick() override;
	virtual void begin() override;

	/// @brief 판정하는 스크립트를 갖고있다면 카운트를 증가시킵니다.
	virtual void BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) override;
	virtual void Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) {}
	/// @brief 판정하는 스크립트를 갖고있다면 카운트를 감소시킵니다.
	virtual void EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) override;

public:
	CLONE(CEventListener);
	CEventListener();
	~CEventListener();

private:
	/// @brief 필요한 컴포넌트들을 모두 보유하는지 검사합니다.
	bool ComponentCheck();
};
