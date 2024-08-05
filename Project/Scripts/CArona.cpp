#include "pch.h"
#include "CArona.h"

#include "CImageUIScript.h"
#include "CStretchUIScript.h"
#include "CTextUI.h"

CArona::CArona()
	: CAtlasImageUIScript((UINT)SCRIPT_TYPE::ARONA)
	, m_vBubbleOffset(100.f, 10.f, 0.f)
	, m_vDisplayPos(-830, 380, 0)
	, m_vHidePos(-1400, 380, 0)
	, m_fTimer(-1.f)
{
	AppendScriptParam("DisplayTimer", SCRIPT_PARAM::FLOAT, &m_fTimer);
}

CArona::~CArona()
{
}
void CArona::begin()
{
	CAtlasImageUIScript::begin();

	// 자식오브젝트들의 명령용 스크립트 불러오기
	const auto& childs = GetOwner()->GetChild();
	for (const auto& child : childs)
	{
		auto pFace = child->GetScript<CImageUIScript>();
		if (pFace)
			m_pFace = pFace;

		auto pBubble = child->GetScript<CStretchUIScript>();
		if (pBubble)
			m_pBubble = pBubble;
		auto pText = child->GetScript<CTextUI>();
		if (pText)
			m_pText = pText;
	}

	MoveToViewportSide();
}

void CArona::tick()
{
	CAtlasImageUIScript::tick();

	// 타이머 도달시 viewport 왼쪽으로 순간이동
	// 타이머 -1 이면 계속 띄우기
	if (m_fTimer == -1.f)
	{
	}
	else
	{
		m_fTimer -= DT;
		if (m_fTimer <= 0.f)
		{
			MoveToViewportSide();
			m_fTimer = -1.f;
		}
	}
}

void CArona::Message(const string& _content, float _size, float _time)
{
	// 1. 텍스트 넣기
	m_pText->SetText(_content);
	// 2. 크기 계산해서 viewport 왼쪽으로 순간이동 3. 말풍선 크기 고려해서 이동
	MoveToViewportSide(_size);
	// 4. lerp 이동(고정위치, 고정시간)
	Transform()->Lerp(m_vDisplayPos, false, Vec3(), false, Vec3(), 0.3f);
	// 5. 타이머 갱신
	m_fTimer = _time;
}

void CArona::MoveToViewportSide(float _size)
{
	Vec3 bubbleScale = m_pBubble->Transform()->GetRelativeScale();
	// 타이머가 종료돼서 이동함
	if (_size == -1.f)
	{
		m_pText->SetText("");
		Transform()->SetRelativePos(m_vHidePos);
		m_pBubble->Transform()->SetRelativePos(m_vBubbleOffset);
		m_pBubble->Transform()->SetRelativeScale(Vec3(50.f, bubbleScale.y, bubbleScale.z));
	}

	// 메시지를 띄우기 위해 이동함
	else
	{
		Transform()->SetRelativePos(m_vHidePos + Vec3(-_size * 2.f, 0.f, 0.f));
		m_pBubble->Transform()->SetRelativePos(m_vBubbleOffset + Vec3(_size / 2.f, 0.f, 0.f));
		m_pBubble->Transform()->SetRelativeScale(Vec3(_size, bubbleScale.y, bubbleScale.z));
	}
}
