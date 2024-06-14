#pragma once
#include "CEntity.h"


#include "CGameObject.h"

class CGameObject;

class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE    m_Type;
    CGameObject* m_Owner;

public:
    virtual void begin() {}
    virtual void tick() {}
    virtual void finaltick() = 0;
    virtual void UpdateData() {};

public:
    COMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }

    inline class CTransform* Transform() { return GetOwner()->Transform();}
    inline class CMeshRender* MeshRender() { return GetOwner()->MeshRender();}
    inline class CCamera* Camera() { return GetOwner()->Camera();}
    inline class CStateMachine* StateMachine() { return GetOwner()->StateMachine();}
    inline class CCollider2D* Collider2D() { return GetOwner()->Collider2D();}
    inline class CAnimator2D* Animator2D() { return GetOwner()->Animator2D(); }
    inline class CAnimator3D* Animator3D() { return GetOwner()->Animator3D();}
    inline class CLight2D* Light2D() { return GetOwner()->Light2D();}
    inline class CLight3D* Light3D() { return GetOwner()->Light3D();}
    inline class CParticleSystem* ParticleSystem() { return GetOwner()->ParticleSystem(); }
    inline class CTileMap* TileMap() { return GetOwner()->TileMap();}
    inline class CSkyBox* SkyBox() { return GetOwner()->SkyBox();}
    inline class CDecal* Decal() { return GetOwner()->Decal();}
    inline class CLandScape* LandScape() { return GetOwner()->LandScape();}

    class CRenderComponent* GetRenderComponent() { return m_Owner->GetRenderComponent(); }
    static bool IsRenderComponent(COMPONENT_TYPE _Type);

    virtual void SaveToFile(FILE* _File) = 0;
    virtual void SaveToFile(ofstream& fout) = 0;
    virtual void LoadFromFile(FILE* _File) = 0;
    virtual void LoadFromFile(ifstream& fin) = 0;

    virtual CComponent* Clone() = 0;
public:
    CComponent(COMPONENT_TYPE _Type);
    CComponent(const CComponent& _OriginComponent);
    ~CComponent();

    friend class CGameObject;
};

