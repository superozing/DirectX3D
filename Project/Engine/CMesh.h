﻿#pragma once
#include "CAsset.h"

#include "CFBXLoader.h"

class CStructuredBuffer;

struct tIndexInfo
{
	ComPtr<ID3D11Buffer> pIB;
	D3D11_BUFFER_DESC	 tIBDesc;
	UINT				 iIdxCount;
	void* pIdxSysMem;
};

class CMesh : public CAsset
{
private:
	ComPtr<ID3D11Buffer> m_VB;
	ComPtr<ID3D11Buffer> m_IB;

	D3D11_BUFFER_DESC m_VBDesc;

	UINT m_VtxCount;

	void* m_VtxSysMem;

	// 하나의 버텍스버퍼에 여러개의 인덱스버퍼가 연결
	vector<tIndexInfo> m_vecIdxInfo;

	// Animation3D 정보
	vector<tMTAnimClip> m_vecAnimClip;
	vector<tMTBone>		m_vecBones;

	CStructuredBuffer* m_pBoneOffset; // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)
	vector<CStructuredBuffer*> m_vecBoneFrameData; // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)

public:
	UINT GetVtxCount() { return m_VtxCount; }
	Vtx* GetVtxSysMem() { return (Vtx*)m_VtxSysMem; }
	UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }
	vector<tIndexInfo> GetvecIndexInfo() { return m_vecIdxInfo; }

	const vector<tMTBone>* GetBones() { return &m_vecBones; }
	UINT					   GetBoneCount() { return (UINT)m_vecBones.size(); }
	const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
	bool					   IsAnimMesh() { return !m_vecAnimClip.empty(); }

	CStructuredBuffer* GetBoneOffsetBuffer() { return m_pBoneOffset; }		   // 각 뼈의 offset 행렬
	vector<CStructuredBuffer*> GetBoneFrameDataBuffer() { return m_vecBoneFrameData; } // 전체 본 프레임 정보

private:
	void UpdateData(UINT _iSubset);
	void UpdateData_Inst(UINT _iSubset);

public:
	static CMesh* CreateFromContainer(CFBXLoader& _loader);
	int			  Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount);
	void		  render(UINT _iSubset);
	void		  render_asparticle(UINT _ParticleCount);
	void		  render_instancing(UINT _iSubset);
	void		  render_structuredbuffer_Instance(UINT _iSubset, int InstanceCount);

private:
	virtual int Load(const wstring& _strFilePath) override;

public:
	virtual int Save(const wstring& _strRelativePath) override;

	CLONE_DISABLE(CMesh);

public:
	CMesh(bool _Engine = false);
	~CMesh();
};
