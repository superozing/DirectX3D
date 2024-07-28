#pragma once
#include "singleton.h"

#include "CTexture.h"

class CGameObject;
class CCamera;
class CLight2D;
class CLight3D;
class CStructuredBuffer;
class CMRT;

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

struct tCromatic_AberrationInfo
{
	bool  Activate		 = false;
	float Duration		 = 10.f;
	float Acctime		 = 0.f;
	Vec2  MaxRedOffSet	 = Vec2(0.f, 0.f);
	Vec2  MaxGreenOffset = Vec2(0.f, 0.f);
	Vec2  MaxBlueOffset	 = Vec2(0.f, 0.f);
	// Vec2  MaxRedOffSet	 = Vec2(-10.f, -10.f);
	// Vec2  MaxGreenOffset = Vec2(10.f, 10.f);
	// Vec2  MaxBlueOffset	 = Vec2(30.f, 30.f);
	//  Vec2  CurRedOffSet	 = {};
	//  Vec2  CurGreenOffset = {};
	//  Vec2  CurBlueOffset	 = {};
	Vec2 CropOffset = Vec2(0.8f, 0.8f);
};

class CRenderMgr : public CManager<CRenderMgr>
{
	SINGLE(CRenderMgr);

private:
	CMRT* m_arrMRT[(UINT)MRT_TYPE::END];

	vector<CCamera*> m_vecCam;
	CCamera*		 m_EditorCam;
	bool			 m_bEscape;

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
	RENDER_FUNC m_RenderFunc;

	Vec4 m_vClearColor;

public:
	bool IsEscape() { return m_bEscape; }

	static void (*CameraChange)(CCamera*);
	/// <summary>
	/// 0번 idx == MainCamera
	/// </summary>
	void RegisterCamera(CCamera* _Cam, int _Idx);
	void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_DbgShapeInfo.push_back(_info); }

	void RePositionDebugCam();

	void SetDebugPosition(bool _OnOff) { m_DebugPosition = _OnOff; }
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

	friend class CRenderMgrScript;
	friend class CDevice;
	friend class CCamera;
};
