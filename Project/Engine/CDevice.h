#pragma once

#include "CTexture.h"

class CConstBuffer;
class CStructuredBuffer;

// Directx11 기준 GPU 제어
class CDevice : public CSingleton<CDevice>
{
	SINGLE(CDevice);

private:
	ComPtr<ID3D11Device>		m_Device;  // GPU 메모리 관리, 객체 생성
	ComPtr<ID3D11DeviceContext> m_Context; // GPU 렌더링 명령

	ComPtr<IDXGISwapChain> m_SwapChain; // 스왚체인(출력 버퍼 지정)

	HWND m_hRenderWnd;
	Vec2 m_vRenderResolution;

	CConstBuffer*	   m_arrCB[(UINT)CB_TYPE::END];
	CStructuredBuffer* m_arrSB[(UINT)SB_TYPE::END];

	ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState> m_arrDS[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BS_TYPE::END];
	ComPtr<ID3D11SamplerState>		m_arrSampler[2];

	bool bIsWindowMode;

public:
	int	 init(HWND _hWnd, Vec2 _vResolution, bool bWindowMode);
	void Present();

	int RenewResolution(Vec2 _vResolutio, bool bWindowMode);

	ID3D11Device*		 GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }
	CConstBuffer*		 GetConstBuffer(CB_TYPE _type) { return m_arrCB[(UINT)_type]; }
	CStructuredBuffer*	 GetStructuredBuffer(SB_TYPE _type) { return m_arrSB[(UINT)_type]; }
	Vec2				 GetRenderResolution() { return m_vRenderResolution; }

	ComPtr<ID3D11RasterizerState>	GetRSState(RS_TYPE _Type) { return m_arrRS[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_arrDS[(UINT)_Type]; }
	ComPtr<ID3D11BlendState>		GetBSState(BS_TYPE _Type) { return m_arrBS[(UINT)_Type]; }

	bool GetScreenMode() { return bIsWindowMode; }
	void SetScreenMode(bool IsFullScreen) { bIsWindowMode = IsFullScreen; }

	void DeleteTexturesForResolutionChange();
	void RematchMtrlTexParam();

	void ReportLiveObjects();

private:
	int CreateSwapChain(bool _bFullscreen = true);
	int CreateTargetView();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSamplerState();

	int CreateConstBuffer();
	int CreateStructuredBuffer();
};
