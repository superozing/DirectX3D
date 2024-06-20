#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRotation;

    Vec3    m_arrLocalDir[3]; // Right, Up, Front
    Vec3    m_arrWorldDir[3]; // Right, Up, Front

    Matrix  m_matWorld; // 상태행렬
    Matrix  m_matWorldInv;  // 월드 역행렬
    bool    m_bAbsolute;
    bool    m_IsDynamic;    // 정적물체 or 동적물체
    bool    m_bDirty;
    //bool    m_FrustumCheck;

    // =========================
    // 부드럽게 움직이기(선형) 위한 변수들
    // =========================
    Vec3 m_vTargetPos;
    Vec3 m_vTargetRot;
    Vec3 m_vTargetScale;
    Vec3 m_vStartPos;
    Vec3 m_vStartRot;
    Vec3 m_vStartScale;
    float m_fTargetTimer;
    float m_fTargetTime;
    bool m_bRotLerp;
    bool m_bScaleLerp;
    bool m_bLerp;

public:
    void Lerp(Vec3 _pos, bool _bMoveRot = false, Vec3 _rot = Vec3(), bool _bMoveScale = false, Vec3 _scale = Vec3(), float _time = 0.f);

public:
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void UpdateData() override;

public:
    void SetRelativePos(Vec3 _Pos) { m_vRelativePos = _Pos; m_bDirty = true; }
    void SetRelativeScale(Vec3 _Scale) { m_vRelativeScale = _Scale; m_bDirty = true; }
    void SetRelativeRotation(Vec3 _Rotation) { m_vRelativeRotation = _Rotation; m_bDirty = true; }

    void SetWorldMat(const Matrix& _matWorld);

    Vec3 GetRelativePos() const { return m_vRelativePos; }
    Vec3 GetRelativeScale() const { return m_vRelativeScale; }
    Vec3 GetRelativeRotation() const { return m_vRelativeRotation; }

    Vec3 GetWorldPos() { return m_matWorld.Translation(); }
    Vec3 GetWorldScale();
    Vec3 GetWorldRot();
    
    void SetDir(Vec3 _Dir);

    void SetAbsolute(bool _bAbsolute) { m_bAbsolute = _bAbsolute; }
    bool IsAbsolute() const { return m_bAbsolute; }

    void SetDynamicObject(bool _Dynamic) { m_IsDynamic = _Dynamic; }
    bool IsDynamic() { return m_IsDynamic; }

    const Matrix& GetWorldMat() 
    { 
        if(m_bDirty)
            CalWorldMat(); 
        m_bDirty = false;
        return m_matWorld; 
    }
    const Matrix& GetWorldInvMat() { return m_matWorldInv; }

    Vec3 GetLocalDir(DIR_TYPE _type) const { return m_arrLocalDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) const { return m_arrWorldDir[(UINT)_type]; }

    Matrix GetWorldViewMatrix() { return g_Transform.matWV; }
    Vec3 GetWorldViewPos() const { return g_Transform.matWV.Translation(); }

    virtual void SaveToFile(FILE* _File) override;
    virtual void SaveToFile(ofstream& fout) override;
    virtual void LoadFromFile(FILE* _File) override;
    virtual void LoadFromFile(ifstream& fin) override;

public:
    CLONE(CTransform);
    CTransform();    
    ~CTransform();

private:
    void CalWorldMat();

};
