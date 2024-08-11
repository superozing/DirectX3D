#pragma once
#include "singleton.h"

#include "CTexture.h"

class CGameObject;
class CCamera;
class CLight2D;
class CLight3D;
class CStructuredBuffer;
class CMRT;

#define VignetteLv1 40.f / 100.f
#define VignetteLv2 60.f / 100.f
#define VignetteLv3 100.f / 100.f

struct tGlobalBloomInfo
{
	bool BloomActivate = true;
	bool GlobalBloom   = false;
	// blur.fx
	int BlurLevel = 3;
	// bloom.fx
	float  Ratio = .14f;
	tBloom GlbalBloomSetting{Vec4(1.f, 1.f, 1.f, 1.f), 0.8f};
};

enum CA_TYPE
{
	CA_TEST,
	CA_SHORT,
	CA_LONG,
};

struct tCromatic_AberrationInfo
{
	CA_TYPE Type		   = CA_TEST;
	bool	Activate	   = false;
	float	Duration	   = 10.f;
	float	RemainTime	   = 0.f;
	Vec2	MaxRedOffSet   = Vec2(-10.f, -10.f);
	Vec2	MaxGreenOffset = Vec2(10.f, 10.f);
	Vec2	MaxBlueOffset  = Vec2(30.f, 30.f);
	Vec2	CropOffset	   = Vec2(0.95f, 0.95f);
};

struct tVignetteInfo
{
	bool  bVignetteRender;
	float fDuration;
	float fAlpha;
	int	  iVignettePower; // 추가 alpha 변동폭
	float fMaxAlpha;
};

class CRenderMgr : public CManager<CRenderMgr>
{
	SINGLE(CRenderMgr);

private:
	CMRT* m_arrMRT[(UINT)MRT_TYPE::END];

	vector<CCamera*> m_vecCam;
	CCamera*		 m_EditorCam;
	bool			 m_bEscape;
	bool			 m_bGlobalShadowRender = true;
	bool			 m_bGlobalPCF		   = true;

	Ptr<CTexture> m_PostProcessTex;

	CStructuredBuffer* m_Light2DBuffer;
	vector<CLight2D*>  m_vecLight2D;

	CStructuredBuffer* m_Light3DBuffer;
	vector<CLight3D*>  m_vecLight3D;

	list<tDebugShapeInfo> m_DbgShapeInfo;
	CGameObject*		  m_pDebugObj;
	bool				  m_DebugPosition;

	// NoiseTexture
	vector<Ptr<CTexture>> m_vecNoiseTex;

	// render function pointer
	typedef void (CRenderMgr::*RENDER_FUNC)(void);
	RENDER_FUNC	 m_RenderFunc;

	Vec4 m_vClearColor;

public:
	bool IsRenderShadow() { return m_bGlobalShadowRender; }
	void RenderShadow(bool _render) { m_bGlobalShadowRender = _render; }
	bool IsPCFShadow() { return m_bGlobalPCF; }
	void PCFShadow(bool _render) { m_bGlobalPCF = _render; }

	bool IsEscape() { return m_bEscape; }

	static void (*CameraChange)(CCamera*);
	/// <summary>
	/// 0번 idx == MainCamera
	/// </summary>
	void RegisterCamera(CCamera* _Cam, int _Idx);
	void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_DbgShapeInfo.push_back(_info); }

	void RePositionDebugCam();

	void SetDebugPosition(bool _OnOff)
	{
		if (_OnOff)
			m_DbgShapeInfo.clear();
		m_DebugPosition = _OnOff;
	}
	bool IsDebugPosition() { return m_DebugPosition; }

	void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }
	int	 RegisterLight3D(CLight3D* _Light3D)
	{
		m_vecLight3D.push_back(_Light3D);
		return m_vecLight3D.size() - 1;
	}

	void		  CopyRenderTargetToPostProcessTarget();
	Ptr<CTexture> GetPostProcessTex() { return m_PostProcessTex; }

	Ptr<CTexture> CopyRTTex(Ptr<CTexture> pTexture);

	void CopyFromTextureToTexture(Ptr<CTexture> pToTexture, Ptr<CTexture> pFromTexture);

	void RegisterEditorCamera(CCamera* _Cam)
	{
		m_EditorCam = _Cam;
		if (CameraChange)
			CameraChange(m_EditorCam);
	}
	void ClearCamera() { m_vecCam.clear(); }

	void ActiveEditorMode(bool _bActive)
	{
		if (_bActive)
			m_RenderFunc = &CRenderMgr::render_editor;
		else
			m_RenderFunc = &CRenderMgr::render_play;
	}

	CMRT* GetMRT(MRT_TYPE _Type) { return m_arrMRT[(UINT)_Type]; }

	const vector<CLight3D*>& GetLight3D() { return m_vecLight3D; }

	void SetClearColor(const Vec4& _ClearColor) { m_vClearColor = _ClearColor; }
	Vec4 GetClearColor() { return m_vClearColor; }

	void CheckEscape();

	CCamera* GetMainCam();
	CCamera* GetEditorCam() { return m_EditorCam; }

	tGlobalBloomInfo		 m_GlobalBloomInfo;
	tCromatic_AberrationInfo m_CAInfo;
	bool					 m_bGrayWeight = true;

	tVignetteInfo m_VignetteInfo;
	bool		  GetRenderVignette() { return m_VignetteInfo.bVignetteRender; }

	float* GetVignetteDuration() { return &m_VignetteInfo.fDuration; }
	float* GetVignetteAlpha() { return &m_VignetteInfo.fAlpha; }
	int*   GetVignettePower() { return &m_VignetteInfo.iVignettePower; }
	void   SetVignettePower(int _Power) { m_VignetteInfo.iVignettePower = _Power; }
	float  GetVignetteMaxAlpha() { return m_VignetteInfo.fMaxAlpha; }

	void SwitchVignette()
	{
		m_VignetteInfo.bVignetteRender = m_VignetteInfo.bVignetteRender ? false : true;

		// 최대 비네트 알파를 정해줄 때 쓰는 값
		int*  iVignettePower = CRenderMgr::GetInst()->GetVignettePower();
		float fMaxAlpha;

		if (*iVignettePower <= 1)
			fMaxAlpha = VignetteLv1;
		else if (*iVignettePower == 2)
			fMaxAlpha = VignetteLv2;
		else if (*iVignettePower > 2)
			fMaxAlpha = VignetteLv3;

		if (m_VignetteInfo.bVignetteRender == true)
		{
			m_VignetteInfo.fDuration = VignetteDuration;
			m_VignetteInfo.fAlpha	 = fMaxAlpha;
			m_VignetteInfo.fMaxAlpha = fMaxAlpha;
		}

		if (m_VignetteInfo.bVignetteRender == false)
		{
			m_VignetteInfo.fDuration = 0.f;
			m_VignetteInfo.fAlpha	 = 0.f;
		}
	}

public:
	virtual void init() override;
	virtual void tick() override;
	virtual void enter() override {}
	virtual void exit() override;

private:
	void CreateDynamicShadowDepth();

	void CreateMRT();
	void ClearMRT();

	void ResetMRT();

	void render_play();
	void render_editor();

	void render_debug();

	// 리소스 바인딩
	void UpdateData();

	// 리소스 클리어
	void Clear();

	// 블러관련
	vector<Ptr<CTexture>> m_vecBlurOneTex;
	vector<Ptr<CTexture>> m_vecBlurTwoTex;
	void				  CreateBlurTex();
	void				  DeleteBlurTex();

private:
	void PushCAEvent()
	{
		m_CAInfo.Activate = true;
		if (CA_TEST == m_CAInfo.Type)
		{
			m_CAInfo.RemainTime = m_CAInfo.Duration;
		}
		else if (CA_SHORT == m_CAInfo.Type)
		{
			m_CAInfo.Duration		= 0.3f;
			m_CAInfo.RemainTime		= m_CAInfo.Duration;
			m_CAInfo.MaxRedOffSet	= Vec2(-5.f, -5.f);
			m_CAInfo.MaxGreenOffset = Vec2(5.f, 5.f);
			m_CAInfo.MaxBlueOffset	= Vec2(15.f, 15.f);
			m_CAInfo.CropOffset		= Vec2(0.95f, 0.95f);
		}
		else if (CA_LONG)
		{
			m_CAInfo.Duration		= 1.f;
			m_CAInfo.RemainTime		= m_CAInfo.Duration;
			m_CAInfo.MaxRedOffSet	= Vec2(-15.f, -15.f);
			m_CAInfo.MaxGreenOffset = Vec2(15.f, 15.f);
			m_CAInfo.MaxBlueOffset	= Vec2(45.f, 45.f);
			m_CAInfo.CropOffset		= Vec2(0.95f, 0.95f);
		}
		else
		{
		}
	}

public:
	void PushCAEvent(CA_TYPE _TYPE)
	{
		m_CAInfo.Type = _TYPE;
		PushCAEvent();
	}

	void PushCAEvent(tCromatic_AberrationInfo _Info)
	{
		m_CAInfo.Activate = true;

		m_CAInfo.RemainTime		= _Info.Duration;
		m_CAInfo.MaxRedOffSet	= _Info.MaxRedOffSet;
		m_CAInfo.MaxGreenOffset = _Info.MaxGreenOffset;
		m_CAInfo.MaxBlueOffset	= _Info.MaxBlueOffset;
	}

	friend class CRenderMgrScript;
	friend class CDevice;
	friend class CCamera;
	friend class SettingUI;
};
