#pragma once
#include "CRenderComponent.h"

class CDecal : public CRenderComponent
{
private:
	int	  m_bAsEmissive;
	int	  m_DecalPriority;
	float m_RenderDistance;
	int	  m_bCustomAlpha;
	float m_fCustomAlpha;

public:
	void SetDecalAsEmissive(bool _bTrue) { m_bAsEmissive = _bTrue; }
	int	 GetDecalAsEmissive() { return m_bAsEmissive; }

	void SetDecalPriority(int _NumPriority) { m_DecalPriority = std::clamp(_NumPriority, 0, DecalPriorityMax); }
	int	 GetDecalPriority() { return m_DecalPriority; }

	void  SetRenderDistance(float _Distance) { m_RenderDistance = _Distance; }
	float GetRenderDistance() { return m_RenderDistance; }

	void  SetUseCustomAlpha(bool _bTrue) { m_bCustomAlpha = _bTrue; }
	int	  GetUsetCustomAlpha() { return m_bCustomAlpha; }
	void  SetCustomAlpha(float _alpha) { m_fCustomAlpha = _alpha; }
	float GetCustomAlpha() { return m_fCustomAlpha; }

public:
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

	virtual void finaltick() override;
	virtual void UpdateData() override;
	virtual void render() override;

public:
	CLONE(CDecal);
	CDecal();
	~CDecal();
};
