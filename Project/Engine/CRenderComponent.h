#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

struct tMtrlSet
{
    Ptr<CMaterial>  pSharedMtrl;    // 공유 메테리얼
    Ptr<CMaterial>  pDynamicMtrl;   // 공유 메테리얼의 복사본    
    Ptr<CMaterial>  pCurMtrl;       // 현재 사용 할 메테리얼
};

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>          m_Mesh;
    vector<tMtrlSet>    m_vecMtrls; // 재질     
    bool                m_DrawShadowMap;

public:
    void SetMesh(Ptr<CMesh> _Mesh);
    void SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx);
    void DrawShadow(bool _bShadow) { m_DrawShadowMap = _bShadow; }

    Ptr<CMesh> GetMesh() { return m_Mesh; }

    Ptr<CMaterial> GetMaterial(UINT _idx);
    Ptr<CMaterial> GetSharedMaterial(UINT _idx);
    Ptr<CMaterial> GetDynamicMaterial(UINT _idx);

    vector<tMtrlSet> GetVecMtrls() { return m_vecMtrls; }

    UINT GetMtrlCount() { return (UINT)m_vecMtrls.size(); }

    bool IsDrawShadow() { return m_DrawShadowMap; }

public:
    virtual void finaltick() {};
    virtual void UpdateData() = 0;
    virtual void render() = 0;


public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void SaveToFile(ofstream& fout) override;
    virtual void LoadFromFile(FILE* _File) override;
    virtual void LoadFromFile(ifstream& fin) override;
    virtual CRenderComponent* Clone() = 0;
public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _OriginRenderCom);
    ~CRenderComponent();
};

