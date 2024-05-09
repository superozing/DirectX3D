#include "pch.h"
#include "CMRT.h"

#include "CDevice.h"


CMRT::CMRT()
	: m_RTView{}
	, m_ClearColor{}
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(Ptr<CTexture>* _pArrTex, UINT _RTCount, Ptr<CTexture> _DSTex)
{
	assert(_RTCount <= 8);

	for (m_RTCount = 0; m_RTCount < _RTCount; ++m_RTCount)
	{
		m_arrRTTex[m_RTCount] = _pArrTex[m_RTCount];
		m_RTView[m_RTCount] = m_arrRTTex[m_RTCount]->GetRTV().Get();
	}

	m_DSTex = _DSTex;
}

void CMRT::SetClearColor(Vec4* _arrClearColor, UINT _RTCount)
{
	for (UINT i = 0; i < _RTCount; ++i)
	{
		m_ClearColor[i] = _arrClearColor[i];
	}
}

void CMRT::OMSet()
{
	// DSTex 사용 여부 체크
	if (nullptr == m_DSTex)
	{
		CONTEXT->OMSetRenderTargets(m_RTCount, m_RTView, nullptr);
	}
	else
	{
		CONTEXT->OMSetRenderTargets(m_RTCount, m_RTView, m_DSTex->GetDSV().Get());
	}
}

void CMRT::Clear()
{
	ClearRT();
	ClearDS();
}

void CMRT::ClearRT()
{
	for (UINT i = 0; i < m_RTCount; ++i)
	{
		CONTEXT->ClearRenderTargetView(m_RTView[i], m_ClearColor[i]);
	}
}

void CMRT::ClearDS()
{
	CONTEXT->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
