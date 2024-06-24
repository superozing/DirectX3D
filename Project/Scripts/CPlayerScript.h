#pragma once

#include <Engine/CScript.h>

enum class PLAYER_STATE
{
	NORMAL,
	ATTACK,
	END,
};

template <typename T>
class CRoRStateMachine;

class CPlayerScript :
    public CScript
{
private:
    Ptr<CPrefab>    m_Missile;
    float           m_Speed;

public:
    virtual void begin() override;
    virtual void tick() override;
	CRoRStateMachine<CPlayerScript> *m_FSM;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CPlayerScript);

    void NormalBegin();
	int NormalUpdate();
	void NormalEnd();
	void AttackBegin();
	int AttackUpdate();
	void AttackEnd();

	public:
    CPlayerScript();
    ~CPlayerScript();
};

