#pragma once
#include "CAtlasImageUIScript.h"
class CArona : public CAtlasImageUIScript
{
private:
	class CImageUIScript*	m_pFace;
	class CStretchUIScript* m_pBubble;
	class CTextUI*			m_pText;

	Vec3 m_vBubbleOffset;
	Vec3 m_vHidePos;
	Vec3 m_vDisplayPos;

	float m_fTimer;

public:
	void Message(const string& _content, float _size, float _time = -1.f);
	void Disappear();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CArona);
	CArona();
	~CArona();

private:
	void MoveToViewportSide(float _size = -1.f);
};
