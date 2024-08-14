#pragma once
#include "CAtlasImageUIScript.h"

enum class FADE_TYPE
{
	FADE_OUT,
	FADE_IN,
};

struct tFadeEvent
{
	FADE_TYPE Type;
	float	  AccTime;
	float	  Duration;
};

class CFadeUIScript : public CAtlasImageUIScript
{
public:
	virtual void begin() override;
	virtual void tick() override;
	float		 m_FadeDuration = 0.f;
	void		 Push_FadeEvent(FADE_TYPE _Type, float _Duration);
	void		 Push_FadeIn() { Push_FadeEvent(FADE_TYPE::FADE_IN, m_FadeDuration); }
	void		 Push_FadeOut() { Push_FadeEvent(FADE_TYPE::FADE_OUT, m_FadeDuration); }

private:
	list<tFadeEvent> m_FadeEventList;

public:
	CLONE(CFadeUIScript);
	CFadeUIScript();
	~CFadeUIScript();
};
