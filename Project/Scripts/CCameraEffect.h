#pragma once
#include <Engine\CScript.h>
class CCameraEffect :
    public CScript
{
private:
    bool m_bShake;
    float m_fShakeTimer;
    float m_fShakeDuration;
    float m_fShakeScale;

private:
    void Shaking();

public:
    void Shake(float _duration, float _scale);

public:
    virtual void tick() override;

public:
    CLONE(CCameraEffect);
    CCameraEffect();
    ~CCameraEffect();
};

