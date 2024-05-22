#pragma once
#include "CRenderComponent.h"

#include "CParticleUpdate.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*      m_ParticleBuffer;
    UINT                    m_MaxParticleCount;

    tParticleModule         m_Module;
    CStructuredBuffer*      m_ParticleModuleBuffer;

    CStructuredBuffer*      m_SpawnCountBuffer;

    Ptr<CParticleUpdate>    m_CSParticleUpdate;

    Ptr<CTexture>           m_ParticleTex;

    float                   m_Time;

public:
    Ptr<CTexture> GetParticleTex() { return m_ParticleTex; }
    void SetParticleTex(Ptr<CTexture> Texture) { m_ParticleTex = Texture; }
    
    int GetMaxParticleCount() { return (int)m_MaxParticleCount; }
    void SetMaxParticleCount(UINT Count) { m_MaxParticleCount = Count; }

    tParticleModule GetParticleModule() { return m_Module; }
    void SetModule(tParticleModule _Module) { m_Module = _Module; }


public:
    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void SaveToFile(ofstream& fout) override;
    virtual void LoadFromFile(FILE* _File) override;
    virtual void LoadFromFile(ifstream& fin) override;

public:
    CLONE(CParticleSystem);
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _OriginParticle);
    ~CParticleSystem();
};

