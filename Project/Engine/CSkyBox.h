#pragma once
#include "CRenderComponent.h"

enum class SKYBOX_TYPE
{
    SPHERE,
    CUBE,
};
class CSkyBox :
    public CRenderComponent
{
private:
    SKYBOX_TYPE     m_SkyBoxType;

    Ptr<CMaterial>  m_SkyBoxMtrl;
    
    Ptr<CTexture>   m_SphereTex;
    Ptr<CTexture>   m_CubeTex;

public:
    void SetSkyBoxType(SKYBOX_TYPE _Type);

    void SetSphereTexture(Ptr<CTexture> _Texture) { m_SphereTex = _Texture; }
    //Ptr<CTexture> GetSphereTex() { return m_SphereTex; }

    void SetCubeTexture(Ptr<CTexture> _Texture) { m_CubeTex = _Texture; }
    Ptr<CTexture> GetCubeTex() { return m_CubeTex; }

    

public:
    virtual void SaveToFile(ofstream& fout) override;
    virtual void LoadFromFile(ifstream& fin) override;

    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;

public:
    CLONE(CSkyBox);
    CSkyBox();
    ~CSkyBox();

    friend class SkyBoxUI;
};

