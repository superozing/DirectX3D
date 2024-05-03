#pragma once
#include <Engine\CScript.h>
class CCameraEffect :
    public CScript
{
private:
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

private:
    void Shaking();

public:
    void Shake(float _duration, Vec2 _scale);

public:
    virtual void tick() override;

public:
    CLONE(CCameraEffect);
    CCameraEffect();
    ~CCameraEffect();
};

