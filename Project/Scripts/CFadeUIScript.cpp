#include "pch.h"
#include "CFadeUIScript.h"

void CFadeUIScript::begin()
{
	CAtlasImageUIScript::begin();
}

void CFadeUIScript::tick()
{
	CAtlasImageUIScript::tick();

	auto& EventList = m_FadeEventList;
	if (EventList.empty())
	{
		auto RelPos = Transform()->GetRelativePos();
		RelPos.x	= 6000.f;
		Transform()->SetRelativePos(RelPos);
		return;
	}

	// Type,LifeTime에 따른 알파값주기
	auto&	  front = EventList.front();
	float	  alpha = 1.f;
	FADE_TYPE type	= front.Type;
	float	  ratio = front.AccTime / front.Duration;
	if (FADE_TYPE::FADE_IN == type)
	{
		alpha = 1.f - ratio;
	}
	else if (FADE_TYPE::FADE_OUT == type)
	{
		alpha = ratio;
	}

	// Pop처리
	front.AccTime += DT;
	if (front.Duration < front.AccTime)
	{
		EventList.pop_front();
	}

	// 랜더하기
	auto color = GetColor();
	color.w	   = alpha * 2.f;
	SetColor(color);
}

#include "Engine\CTransform.h"
void CFadeUIScript::Push_FadeEvent(FADE_TYPE _Type, float _Duration)
{
	tFadeEvent Event = {};
	Event.Type		 = _Type;
	Event.Duration	 = _Duration;
	m_FadeEventList.push_back(Event);

	auto RelPos = Transform()->GetRelativePos();
	RelPos.x	= 0.f;
	Transform()->SetRelativePos(RelPos);
}

CFadeUIScript::CFadeUIScript()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::FADEUISCRIPT)
{
}

CFadeUIScript::~CFadeUIScript()
{
}
