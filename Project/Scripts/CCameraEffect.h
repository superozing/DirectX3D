#pragma once
#include <Engine\CScript.h>
class CCameraEffect :
    public CScript
{
private:
    Vec3 m_vOriginPos;
    Vec3 m_vOriginRot;

    bool m_bShake;
    float m_fShakeTimer;
    float m_fShakeDuration;
    Vec2 m_vShakePosIntensity;
    Vec3 m_vShakeRotationIntensity;

    float m_fShakeFrequencyTimer;
    float m_fShakeFrequency;
    Vec3 m_vStartPos;
    Vec3 m_vTargetPos;
    Vec3 m_vStartRotation;
    Vec3 m_vTargetRotation;

    Vec3 m_vInitialPos;
    Vec3 m_vInitialRotation;

private:
    void Shaking();

public:
    void Shake(float _duration, Vec2 _scale);
    void RegistInitial();
    void RegistInitial(Vec3 _pos, Vec3 _rot);
    void SendToInitial();

public:
    virtual void tick() override;
    virtual void begin() override;

public:
    CLONE(CCameraEffect);
    CCameraEffect();
    ~CCameraEffect();
};

