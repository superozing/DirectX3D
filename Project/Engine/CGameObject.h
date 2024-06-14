#pragma once
#include "CEntity.h"

class CComponent;
class CScript;

class CGameObject :
    public CEntity
{
private:
    CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];
    class CRenderComponent* m_RenderCom;

    vector<CScript*>        m_vecScript;
    vector<CGameObject*>    m_vecChild;

    CGameObject* m_Parent;

    int                     m_iLayerIdx;    // 오브젝트가 소속되어있는 Layer 의 Index

    bool                    m_bDead;

public:
    void begin();
    void tick();
    virtual void finaltick();
    void render();

public:
    void AddComponent(CComponent* _Component);
    void DeleteComponent(COMPONENT_TYPE _Type);
    void DeleteScript(CScript* _Script);

    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }
    class CRenderComponent* GetRenderComponent() { return m_RenderCom; }

    inline class CTransform* Transform() { return (CTransform*)m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]; }
    inline class CMeshRender* MeshRender() { return (CMeshRender*)m_arrCom[(UINT)COMPONENT_TYPE::MESHRENDER]; }
    inline class CCamera* Camera() { return (CCamera*)m_arrCom[(UINT)COMPONENT_TYPE::CAMERA]; }
    inline class CStateMachine* StateMachine() { return (CStateMachine*)m_arrCom[(UINT)COMPONENT_TYPE::STATEMACHINE]; }
    inline class CCollider2D* Collider2D() { return (CCollider2D*)m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER2D]; }
    inline class CAnimator2D* Animator2D() { return (CAnimator2D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR2D]; }
    inline class CAnimator3D* Animator3D() { return (CAnimator3D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR3D]; }
    inline class CLight2D* Light2D() { return (CLight2D*)m_arrCom[(UINT)COMPONENT_TYPE::LIGHT2D]; }
    inline class CLight3D* Light3D() { return (CLight3D*)m_arrCom[(UINT)COMPONENT_TYPE::LIGHT3D]; }
    inline class CParticleSystem* ParticleSystem() { return (CParticleSystem*)m_arrCom[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]; }
    inline class CTileMap* TileMap() { return (CTileMap*)m_arrCom[(UINT)COMPONENT_TYPE::TILEMAP]; }
    inline class CSkyBox* SkyBox() { return (CSkyBox*)m_arrCom[(UINT)COMPONENT_TYPE::SKYBOX]; }
    inline class CDecal* Decal() { return (CDecal*)m_arrCom[(UINT)COMPONENT_TYPE::DECAL]; }
    inline class CLandScape* LandScape() { return (CLandScape*)m_arrCom[(UINT)COMPONENT_TYPE::LANDSCAPE]; }

    int GetLayerIdx() { return m_iLayerIdx; }

    CGameObject* GetParent() { return m_Parent; }
    const vector<CGameObject*>& GetChild() { return m_vecChild; }

    const vector<CScript*>& GetScripts() { return m_vecScript; }

    template<typename T>
    T* GetScript()
    {
        for (size_t i = 0; i < m_vecScript.size(); ++i)
        {
            if (dynamic_cast<T*>(m_vecScript[i]))
                return (T*)m_vecScript[i];
        }
        return nullptr;
    }


    int DisconnectWithParent();
    int DisconnectWithLayer();

    void AddChild(CGameObject* _Child);
    bool IsDead() { return m_bDead; }

    void Destroy();

    bool IsAncestor(CGameObject* _Other);




    CLONE(CGameObject);
public:
    CGameObject();
    CGameObject(const CGameObject& _OriginObject);
    ~CGameObject();

    friend class CLevel;
    friend class CLayer;
    friend class CTaskMgr;
};
