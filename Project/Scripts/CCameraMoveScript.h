#pragma once
#include <Engine/CScript.h>

class CCameraMoveScript :
    public CScript
{
private:
    float       m_CamSpeed;
    float m_fZoomSpeed;

private:
    void MoveOrthographic();
    void MovePerspective();

public:
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override {};
    virtual void LoadFromFile(FILE* _File) override {};

public:
    CLONE(CCameraMoveScript);
    CCameraMoveScript();
    ~CCameraMoveScript();
};

