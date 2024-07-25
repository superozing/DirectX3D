#pragma once
#include "CRenderComponent.h"

class CDecal : public CRenderComponent
{
private:
	// Decal 기본 설정
	int	  m_bAsEmissive;
	int	  m_DecalPriority;
	float m_RenderDistance;
	int	  m_bCustomAlpha;
	float m_fCustomAlpha;
	float m_fAnimationActionTime;
	float m_OriginAnimationActionTime;

	// animation 설정
	Vec4 m_AnimationOutlinerColor;
	Vec4 m_AnimationInsideColor;
	Vec4 m_AnimationOutsideColor;

	// info
	DecalShape m_DecalShape;
	DecalType  m_DecalType;

public:
	void SetDecalAsEmissive(bool _bTrue) { m_bAsEmissive = _bTrue; }
	int	 GetDecalAsEmissive() { return m_bAsEmissive; }

	void SetDecalPriority(int _NumPriority) { m_DecalPriority = std::clamp(_NumPriority, 0, DecalPriorityMax); }
	int	 GetDecalPriority() { return m_DecalPriority; }

	void  SetRenderDistance(float _Distance) { m_RenderDistance = _Distance; }
	float GetRenderDistance() { return m_RenderDistance; }

	void  SetUseCustomAlpha(bool _bTrue) { m_bCustomAlpha = _bTrue; }
	int	  GetUseCustomAlpha() { return m_bCustomAlpha; }
	void  SetCustomAlpha(float _alpha) { m_fCustomAlpha = _alpha; }
	float GetCustomAlpha() { return m_fCustomAlpha; }

	void ChangeMtrl(wstring _MtrlKey);

	DecalShape GetDecalShape() { return m_DecalShape; }
	void	   SetDecalShape(DecalShape _Shape) { m_DecalShape = _Shape; }

	DecalType GetDecalType() { return m_DecalType; }
	void	  SetDecalType(DecalType _Type) { m_DecalType = _Type; }

	void  SetAnimationActionTime(float _Time) { m_fAnimationActionTime = _Time; }
	float GetAnimationActionTime() { return m_fAnimationActionTime; }

	void  SetAnimationActionOriginTime(float _Time) { m_OriginAnimationActionTime = _Time; }
	float GetAnimationActionOriginTime() { return m_OriginAnimationActionTime; }

	void SyncTime() { m_fAnimationActionTime = m_OriginAnimationActionTime; }

	Vec4 GetAnimationOutlinerColor() { return m_AnimationOutlinerColor; }
	Vec4 GetAnimationInsideColor() { return m_AnimationInsideColor; }
	Vec4 GetAnimationOutsideColor() { return m_AnimationOutsideColor; }

	void SetAnimationOutlinerColor(Vec4 _Color) { m_AnimationOutlinerColor = _Color; }
	void SetAnimationInsideColor(Vec4 _Color) { m_AnimationInsideColor = _Color; }
	void SetAnimationOutsideColor(Vec4 _Color) { m_AnimationOutsideColor = _Color; }

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
