﻿#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;

class CAnimator3D : public CComponent
{
private:
	const vector<tMTBone>*	   m_pVecBones;
	const vector<tMTAnimClip>* m_pVecClip;

	vector<float>  m_vecClipUpdateTime;
	vector<Matrix> m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보

	int	   m_iFrameCount; // 30
	double m_dCurTime;
	int	   m_iCurClip; // 클립 인덱스

	int	  m_iFrameIdx;	   // 클립의 현재 프레임
	int	  m_iNextFrameIdx; // 클립의 다음 프레임
	float m_fRatio;		   // 프레임 사이 비율

	CStructuredBuffer* m_pBoneFrameMatBuffer; // 본 프레임 행렬정보
	CStructuredBuffer* m_pBoneFinalMatBuffer; // 특정 프레임의 최종 행렬
	bool			   m_bFinalMatUpdate;	  // 최종행렬 연산 수행여부

	bool m_bPlay; // 애니메이션 재생 여부
	int m_iLoopCount; // 애니메이션 반복 횟수 (-1 : 무한반복, 0 : 한번만 재생, 1~ : 수치만큼 추가 반복 재생)
	int m_iCurLoopCount;

public:
	virtual void finaltick() override;
	void		 UpdateData();

public:
	void SetBones(const vector<tMTBone>* _vecBones)
	{
		m_pVecBones = _vecBones;
		m_vecFinalBoneMat.resize(m_pVecBones->size());
	}
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

	CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
	vector<Matrix>	   GetVecBoneFrameMat() { return m_vecFinalBoneMat; }

	UINT				   GetBoneCount() { return (UINT)m_pVecBones->size(); }
	const vector<tMTBone>* GetBones() { return m_pVecBones; }
	void				   ClearData();

public:
	const vector<tMTAnimClip>* GetAnimClip() { return m_pVecClip; }

	void SetCurClip(int _Clip) { m_iCurClip = _Clip; }
	int	 GetCurClip() { return m_iCurClip; }

	int GetCurFrameIdx() { return m_iFrameIdx; }

	void SetPlayable(bool _bPlay) { m_bPlay = _bPlay; }
	bool IsPlayable() { return m_bPlay; }

	void SetLoopCount(int _LoopCount) { m_iLoopCount = _LoopCount; }
	int	 GetLoopCount() { return m_iLoopCount; }

	vector<float> GetClipUpdateTime() { return m_vecClipUpdateTime; }
	float		  GetClipUpdateTime(int _idx) { return m_vecClipUpdateTime[_idx]; }

public:
	void Play(int _iClipIdx, int _iLoopCount = -1);
	void Play(const wstring& _AnimName, int _iLoopCount = -1);
	void Stop();

	Matrix FindBoneMat(int _idx);
	Matrix FindBoneMat(const wstring& _strBoneName);

private:
	void check_mesh(Ptr<CMesh> _pMesh);

public:
	virtual void SaveToFile(FILE* _pFile) override;
	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(FILE* _pFile) override;
	virtual void LoadFromFile(ifstream& fin) override;
	CLONE(CAnimator3D);

public:
	CAnimator3D();
	CAnimator3D(const CAnimator3D& _origin);
	~CAnimator3D();
};