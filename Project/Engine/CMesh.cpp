﻿#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"
#include "CInstancingBuffer.h"

CMesh::CMesh(bool _Engine)
	: CAsset(ASSET_TYPE::MESH, _Engine)
	, m_VtxCount(0)
	, m_VtxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
	if (nullptr != m_VtxSysMem)
		delete m_VtxSysMem;

	for (size_t i = 0; i < m_vecIdxInfo.size(); ++i)
	{
		if (nullptr != m_vecIdxInfo[i].pIdxSysMem)
			delete m_vecIdxInfo[i].pIdxSysMem;
	}

	if (nullptr != m_pBoneOffset)
		delete m_pBoneOffset;

	Delete_Vec(m_vecBoneFrameData);
}

CMesh* CMesh::CreateFromContainer(CFBXLoader& _loader)
{
	const tContainer* container = &_loader.GetContainer(0);

	UINT iVtxCount = (UINT)container->vecPos.size();

	D3D11_BUFFER_DESC tVtxDesc = {};

	tVtxDesc.ByteWidth = sizeof(Vtx) * iVtxCount;
	tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
	if (D3D11_USAGE_DYNAMIC == tVtxDesc.Usage)
		tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem				= malloc(tVtxDesc.ByteWidth);
	Vtx* pSys					= (Vtx*)tSub.pSysMem;
	for (UINT i = 0; i < iVtxCount; ++i)
	{
		pSys[i].vPos	  = container->vecPos[i];
		pSys[i].vUV		  = container->vecUV[i];
		pSys[i].vColor	  = Vec4(1.f, 0.f, 1.f, 1.f);
		pSys[i].vNormal	  = container->vecNormal[i];
		pSys[i].vTangent  = container->vecTangent[i];
		pSys[i].vBinormal = container->vecBinormal[i];
		pSys[i].vWeights  = container->vecWeights[i];
		pSys[i].vIndices  = container->vecIndices[i];
	}

	ComPtr<ID3D11Buffer> pVB = NULL;
	if (FAILED(DEVICE->CreateBuffer(&tVtxDesc, &tSub, pVB.GetAddressOf())))
	{
		return NULL;
	}

	CMesh* pMesh	   = new CMesh;
	pMesh->m_VB		   = pVB;
	pMesh->m_VBDesc	   = tVtxDesc;
	pMesh->m_VtxSysMem = pSys;

	// 인덱스 정보
	UINT			  iIdxBufferCount = (UINT)container->vecIdx.size();
	D3D11_BUFFER_DESC tIdxDesc		  = {};

	for (UINT i = 0; i < iIdxBufferCount; ++i)
	{
		tIdxDesc.ByteWidth = (UINT)container->vecIdx[i].size() * sizeof(UINT); // Index Format 이 R32_UINT 이기 때문
		tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		tIdxDesc.Usage	   = D3D11_USAGE_DEFAULT;
		if (D3D11_USAGE_DYNAMIC == tIdxDesc.Usage)
			tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		void* pSysMem = malloc(tIdxDesc.ByteWidth);
		memcpy(pSysMem, container->vecIdx[i].data(), tIdxDesc.ByteWidth);
		tSub.pSysMem = pSysMem;

		ComPtr<ID3D11Buffer> pIB = nullptr;
		if (FAILED(DEVICE->CreateBuffer(&tIdxDesc, &tSub, pIB.GetAddressOf())))
		{
			return NULL;
		}

		tIndexInfo info = {};
		info.tIBDesc	= tIdxDesc;
		info.iIdxCount	= (UINT)container->vecIdx[i].size();
		info.pIdxSysMem = pSysMem;
		info.pIB		= pIB;

		pMesh->m_vecIdxInfo.push_back(info);
	}

	// Animation3D
	if (!container->bAnimation)
		return pMesh;
	vector<tAnimClip*>& vecAnimClip = _loader.GetAnimClip();

	vector<tBone*>& vecBone		= _loader.GetBones();
	UINT			iFrameCount = 0;
	for (UINT i = 0; i < vecBone.size(); ++i)
	{
		tMTBone bone	 = {};
		bone.iDepth		 = vecBone[i]->iDepth;
		bone.iParentIndx = vecBone[i]->iParentIndx;
		bone.matBone	 = GetMatrixFromFbxMatrix(vecBone[i]->matBone);
		bone.matOffset	 = GetMatrixFromFbxMatrix(vecBone[i]->matOffset);
		bone.strBoneName = vecBone[i]->strBoneName;
		bone.vecKeyFrame.resize(vecAnimClip.size());
		for (UINT j = 0; j < vecBone[i]->vecKeyFrame.size(); ++j)
		{
			for (size_t k = 0; k < vecBone[i]->vecKeyFrame[j].size(); ++k)
			{
				tMTKeyFrame tKeyframe  = {};
				tKeyframe.dTime		   = vecBone[i]->vecKeyFrame[j][k].dTime;
				tKeyframe.iFrame	   = j;
				tKeyframe.vTranslate.x = (float)vecBone[i]->vecKeyFrame[j][k].matTransform.GetT().mData[0];
				tKeyframe.vTranslate.y = (float)vecBone[i]->vecKeyFrame[j][k].matTransform.GetT().mData[1];
				tKeyframe.vTranslate.z = (float)vecBone[i]->vecKeyFrame[j][k].matTransform.GetT().mData[2];

				tKeyframe.vScale.x = (float)vecBone[i]->vecKeyFrame[j][k].matTransform.GetS().mData[0];
				tKeyframe.vScale.y = (float)vecBone[i]->vecKeyFrame[j][k].matTransform.GetS().mData[1];
				tKeyframe.vScale.z = (float)vecBone[i]->vecKeyFrame[j][k].matTransform.GetS().mData[2];

				tKeyframe.qRot.x = (float)vecBone[i]->vecKeyFrame[j][k].matTransform.GetQ().mData[0];
				tKeyframe.qRot.y = (float)vecBone[i]->vecKeyFrame[j][k].matTransform.GetQ().mData[1];
				tKeyframe.qRot.z = (float)vecBone[i]->vecKeyFrame[j][k].matTransform.GetQ().mData[2];
				tKeyframe.qRot.w = (float)vecBone[i]->vecKeyFrame[j][k].matTransform.GetQ().mData[3];

				bone.vecKeyFrame[j].push_back(tKeyframe);
			}
		}

		// 각 클립에서 최대 프레임 수를 계산
		for (const auto& keyFrames : bone.vecKeyFrame)
		{
			iFrameCount = max(iFrameCount, (UINT)keyFrames.size());
		}

		pMesh->m_vecBones.push_back(bone);
	}

	for (UINT i = 0; i < vecAnimClip.size(); ++i)
	{
		tMTAnimClip tClip = {};

		tClip.strAnimName = vecAnimClip[i]->strName;
		tClip.dStartTime  = vecAnimClip[i]->tStartTime.GetSecondDouble();
		tClip.dEndTime	  = vecAnimClip[i]->tEndTime.GetSecondDouble();
		tClip.dTimeLength = tClip.dEndTime - tClip.dStartTime;

		tClip.iStartFrame  = (int)vecAnimClip[i]->tStartTime.GetFrameCount(vecAnimClip[i]->eMode);
		tClip.iEndFrame	   = (int)vecAnimClip[i]->tEndTime.GetFrameCount(vecAnimClip[i]->eMode);
		tClip.iFrameLength = tClip.iEndFrame - tClip.iStartFrame;
		tClip.eMode		   = vecAnimClip[i]->eMode;

		pMesh->m_vecAnimClip.push_back(tClip);
	}

	if (pMesh->IsAnimMesh())
	{
		vector<Matrix> vecOffset;

		// 오프셋 데이터는 한 번만 생성
		for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
		{
			vecOffset.push_back(pMesh->m_vecBones[i].matOffset);
		}

		if (nullptr == pMesh->m_pBoneOffset)
		{
			pMesh->m_pBoneOffset = new CStructuredBuffer;
			pMesh->m_pBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), SB_READ_TYPE::READ_ONLY, false,
										 vecOffset.data());
		}

		// 애니메이션 클립 수만큼 반복하면서 본 프레임 데이터를 생성
		for (size_t clipIdx = 0; clipIdx < pMesh->m_vecAnimClip.size(); ++clipIdx)
		{
			vector<tFrameTrans> vecFrameTrans;
			UINT				iFrameCount = (UINT)pMesh->m_vecAnimClip[clipIdx].iFrameLength;
			vecFrameTrans.resize((UINT)pMesh->m_vecBones.size() * iFrameCount);

			for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
			{
				for (size_t frameIdx = 0; frameIdx < pMesh->m_vecBones[i].vecKeyFrame[clipIdx].size(); ++frameIdx)
				{
					vecFrameTrans[(UINT)pMesh->m_vecBones.size() * frameIdx + i] =
						tFrameTrans{Vec4(pMesh->m_vecBones[i].vecKeyFrame[clipIdx][frameIdx].vTranslate, 0.f),
									Vec4(pMesh->m_vecBones[i].vecKeyFrame[clipIdx][frameIdx].vScale, 0.f),
									pMesh->m_vecBones[i].vecKeyFrame[clipIdx][frameIdx].qRot};
				}
			}

			CStructuredBuffer* pSB = new CStructuredBuffer;
			pSB->Create(sizeof(tFrameTrans), (UINT)vecFrameTrans.size(), SB_READ_TYPE::READ_ONLY, true,
						vecFrameTrans.data());
			pMesh->m_vecBoneFrameData.push_back(pSB);
		}
	}

	return pMesh;
}

int CMesh::Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount)
{
	m_VtxCount = _VtxCount;

	// 버텍스 버퍼 생성
	m_VBDesc = {};

	m_VBDesc.ByteWidth			 = sizeof(Vtx) * _VtxCount;
	m_VBDesc.StructureByteStride = sizeof(Vtx);
	m_VBDesc.BindFlags			 = D3D11_BIND_VERTEX_BUFFER;

	// 버퍼에 데이터 쓰기 가능
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.Usage			= D3D11_USAGE_DEFAULT;

	// g_Vtx 배열의 데이터를 초기 데이터로 설정
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem				= _Vtx;

	// 버텍스 버퍼 생성
	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSubData, m_VB.GetAddressOf())))
	{
		MessageBox(nullptr, L"버텍스 버퍼 생성 실패", L"오류", MB_OK);
		return E_FAIL;
	}

	// 인덱스 버퍼 생성
	tIndexInfo IndexInfo = {};
	IndexInfo.iIdxCount	 = _IdxCount;

	IndexInfo.tIBDesc.ByteWidth = sizeof(UINT) * _IdxCount;

	// 버퍼 생성 이후에도, 버퍼의 내용을 수정 할 수 있는 옵션
	IndexInfo.tIBDesc.CPUAccessFlags = 0;
	IndexInfo.tIBDesc.Usage			 = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	// 정점을 저장하는 목적의 버퍼 임을 알림
	IndexInfo.tIBDesc.BindFlags			  = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	IndexInfo.tIBDesc.MiscFlags			  = 0;
	IndexInfo.tIBDesc.StructureByteStride = 0;

	// 초기 데이터를 넘겨주기 위한 정보 구조체
	tSubData.pSysMem = _Idx;

	if (FAILED(DEVICE->CreateBuffer(&IndexInfo.tIBDesc, &tSubData, IndexInfo.pIB.GetAddressOf())))
	{
		assert(nullptr);
	}

	// 원본 정점정보 및 인덱스 정보를 동적할당한 곳에다가 저장시켜두고 관리
	m_VtxSysMem			 = new Vtx[m_VtxCount];
	IndexInfo.pIdxSysMem = new UINT[IndexInfo.iIdxCount];

	memcpy(m_VtxSysMem, _Vtx, sizeof(Vtx) * m_VtxCount);
	memcpy(IndexInfo.pIdxSysMem, _Idx, sizeof(UINT) * IndexInfo.iIdxCount);

	m_vecIdxInfo.push_back(IndexInfo);

	return S_OK;
}

void CMesh::UpdateData(UINT _iSubset)
{
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubset].pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::UpdateData_Inst(UINT _iSubset)
{
	if (_iSubset >= m_vecIdxInfo.size())
		assert(nullptr);

	ID3D11Buffer* arrBuffer[2] = {m_VB.Get(), CInstancingBuffer::GetInst()->GetBuffer().Get()};
	UINT		  iStride[2]   = {sizeof(Vtx), sizeof(tInstancingData)};
	UINT		  iOffset[2]   = {0, 0};

	CONTEXT->IASetVertexBuffers(0, 2, arrBuffer, iStride, iOffset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubset].pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render(UINT _iSubset)
{
	UpdateData(_iSubset);

	CONTEXT->DrawIndexed(m_vecIdxInfo[_iSubset].iIdxCount, 0, 0);
}

void CMesh::render_asparticle(UINT _ParticleCount)
{
	UpdateData(0);

	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[0].iIdxCount, _ParticleCount, 0, 0, 0);
}

void CMesh::render_instancing(UINT _iSubset)
{
	UpdateData_Inst(_iSubset);

	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[_iSubset].iIdxCount, CInstancingBuffer::GetInst()->GetInstanceCount(), 0,
								  0, 0);
}

int CMesh::Save(const wstring& _strRelativePath)
{
	// 상대경로 저장
	SetRelativePath(_strRelativePath);

	// 파일 경로 만들기
	wstring strFilePath = CPathMgr::GetContentPath() + _strRelativePath;

	// 파일 쓰기모드로 열기
	FILE*	pFile = nullptr;
	errno_t err	  = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// 키값, 상대 경로
	SaveWString(GetName(), pFile);
	SaveWString(GetKey(), pFile);
	SaveWString(GetRelativePath(), pFile);

	// 정점 데이터 저장
	int iByteSize = m_VBDesc.ByteWidth;
	fwrite(&iByteSize, sizeof(int), 1, pFile);
	fwrite(m_VtxSysMem, iByteSize, 1, pFile);

	// 인덱스 정보
	UINT iMtrlCount = (UINT)m_vecIdxInfo.size();
	fwrite(&iMtrlCount, sizeof(int), 1, pFile);

	UINT iIdxBuffSize = 0;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		fwrite(&m_vecIdxInfo[i], sizeof(tIndexInfo), 1, pFile);
		fwrite(m_vecIdxInfo[i].pIdxSysMem, m_vecIdxInfo[i].iIdxCount * sizeof(UINT), 1, pFile);
	}

	// Animation3D 정보
	UINT iCount = (UINT)m_vecAnimClip.size();
	fwrite(&iCount, sizeof(int), 1, pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		SaveWString(m_vecAnimClip[i].strAnimName, pFile);
		fwrite(&m_vecAnimClip[i].dStartTime, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].dEndTime, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].dTimeLength, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].eMode, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].fUpdateTime, sizeof(float), 1, pFile);
		fwrite(&m_vecAnimClip[i].iStartFrame, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].iEndFrame, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].iFrameLength, sizeof(int), 1, pFile);
	}

	iCount = (UINT)m_vecBones.size();
	fwrite(&iCount, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iCount; ++i)
	{
		SaveWString(m_vecBones[i].strBoneName, pFile);
		fwrite(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fwrite(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		int iFrameCount = (int)m_vecBones[i].vecKeyFrame.size();
		fwrite(&iFrameCount, sizeof(int), 1, pFile);

		for (int j = 0; j < (int)m_vecBones[i].vecKeyFrame.size(); ++j)
		{
			int jFrameCount = (int)m_vecBones[i].vecKeyFrame[j].size();
			fwrite(&jFrameCount, sizeof(int), 1, pFile);

			for (int k = 0; k < jFrameCount; ++k)
			{
				fwrite(&m_vecBones[i].vecKeyFrame[j][k], sizeof(tMTKeyFrame), 1, pFile);
			}
		}
	}

	fclose(pFile);

	return S_OK;
}

int CMesh::Load(const wstring& _strFilePath)
{
	// 읽기모드로 파일열기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	// 키값, 상대경로
	wstring strName, strKey, strRelativePath;
	LoadWString(strName, pFile);
	LoadWString(strKey, pFile);
	LoadWString(strRelativePath, pFile);

	SetName(strName);
	SetKey(strKey);
	SetRelativePath(strRelativePath);

	// 정점데이터
	UINT iByteSize = 0;
	fread(&iByteSize, sizeof(int), 1, pFile);

	m_VtxSysMem = (Vtx*)malloc(iByteSize);
	fread(m_VtxSysMem, 1, iByteSize, pFile);

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	tDesc.ByteWidth			= iByteSize;
	tDesc.Usage				= D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem				= m_VtxSysMem;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tSubData, m_VB.GetAddressOf())))
	{
		assert(nullptr);
	}

	// 인덱스 정보
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		tIndexInfo info = {};
		fread(&info, sizeof(tIndexInfo), 1, pFile);

		UINT iByteWidth = info.iIdxCount * sizeof(UINT);

		void* pSysMem	= malloc(iByteWidth);
		info.pIdxSysMem = pSysMem;
		fread(info.pIdxSysMem, iByteWidth, 1, pFile);

		tSubData.pSysMem = info.pIdxSysMem;

		if (FAILED(DEVICE->CreateBuffer(&info.tIBDesc, &tSubData, info.pIB.GetAddressOf())))
		{
			assert(nullptr);
		}

		m_vecIdxInfo.push_back(info);
	}

	// Animation3D 정보 읽기
	int iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	for (int i = 0; i < iCount; ++i)
	{
		tMTAnimClip tClip = {};

		LoadWString(tClip.strAnimName, pFile);
		fread(&tClip.dStartTime, sizeof(double), 1, pFile);
		fread(&tClip.dEndTime, sizeof(double), 1, pFile);
		fread(&tClip.dTimeLength, sizeof(double), 1, pFile);
		fread(&tClip.eMode, sizeof(int), 1, pFile);
		fread(&tClip.fUpdateTime, sizeof(float), 1, pFile);
		fread(&tClip.iStartFrame, sizeof(int), 1, pFile);
		fread(&tClip.iEndFrame, sizeof(int), 1, pFile);
		fread(&tClip.iFrameLength, sizeof(int), 1, pFile);

		m_vecAnimClip.push_back(tClip);
	}

	iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	m_vecBones.resize(iCount);

	UINT _iFrameCount = 0;
	for (int i = 0; i < iCount; ++i)
	{
		LoadWString(m_vecBones[i].strBoneName, pFile);
		fread(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fread(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		UINT iFrameCount = 0;
		fread(&iFrameCount, sizeof(int), 1, pFile);
		m_vecBones[i].vecKeyFrame.resize(iFrameCount);

		for (UINT j = 0; j < iFrameCount; ++j)
		{
			UINT jFrameCount = 0;
			fread(&jFrameCount, sizeof(int), 1, pFile);
			m_vecBones[i].vecKeyFrame[j].resize(jFrameCount);
			_iFrameCount = max(_iFrameCount, iFrameCount);

			for (size_t k = 0; k < jFrameCount; ++k)
			{
				fread(&m_vecBones[i].vecKeyFrame[j][k], sizeof(tMTKeyFrame), 1, pFile);
			}
		}
	}

	if (m_vecAnimClip.size() > 0 && m_vecBones.size() > 0)
	{
		vector<Matrix> vecOffset;

		// 오프셋 데이터는 한 번만 생성
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i].matOffset);
		}

		if (nullptr == m_pBoneOffset)
		{
			m_pBoneOffset = new CStructuredBuffer;
			m_pBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), SB_READ_TYPE::READ_ONLY, false,
								  vecOffset.data());
		}

		// 애니메이션 클립 수만큼 반복하면서 본 프레임 데이터를 생성
		for (size_t clipIdx = 0; clipIdx < m_vecAnimClip.size(); ++clipIdx)
		{
			vector<tFrameTrans> vecFrameTrans;
			UINT				iFrameCount = (UINT)m_vecAnimClip[clipIdx].iFrameLength;
			vecFrameTrans.resize((UINT)m_vecBones.size() * iFrameCount);

			for (size_t i = 0; i < m_vecBones.size(); ++i)
			{
				for (size_t frameIdx = 0; frameIdx < m_vecBones[i].vecKeyFrame[clipIdx].size(); ++frameIdx)
				{
					vecFrameTrans[(UINT)m_vecBones.size() * frameIdx + i] =
						tFrameTrans{Vec4(m_vecBones[i].vecKeyFrame[clipIdx][frameIdx].vTranslate, 0.f),
									Vec4(m_vecBones[i].vecKeyFrame[clipIdx][frameIdx].vScale, 0.f),
									m_vecBones[i].vecKeyFrame[clipIdx][frameIdx].qRot};
				}
			}

			CStructuredBuffer* pSB = new CStructuredBuffer;
			pSB->Create(sizeof(tFrameTrans), (UINT)vecFrameTrans.size(), SB_READ_TYPE::READ_ONLY, true,
						vecFrameTrans.data());
			m_vecBoneFrameData.push_back(pSB);
		}
	}

	fclose(pFile);

	return S_OK;
}