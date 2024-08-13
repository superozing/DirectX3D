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
	void		 Push_FadeEvent(FADE_TYPE _Type, float _Duration);

private:
	list<tFadeEvent> m_FadeEventList;

public:
	CLONE(CFadeUIScript);
	CFadeUIScript();
	~CFadeUIScript();
};
