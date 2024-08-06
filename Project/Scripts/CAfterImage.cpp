#include "pch.h"
#include "CAfterImage.h"

#include <Engine\CDevice.h>
#include <Engine\CStructuredBuffer.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>

#include <Engine\CAnimator3D.h>

#include <Engine\CRenderComponent.h>

CAfterImage::CAfterImage()
	: CScript((UINT)SCRIPT_TYPE::AFTERIMAGE)
{

	// 디폴트 세팅

	m_info.NodeCount = 10;
	m_info.TimeStep	 = 0.1f;
	fUpdateTimer	 = 0.1f;
	bDisplayNode	 = true;

	AppendScriptParam("Node Count", SCRIPT_PARAM::INT, &m_info.NodeCount, 0, 10, false, "Afterimage Node count");
	AppendScriptParam("Time Interval", SCRIPT_PARAM::FLOAT, &m_info.TimeStep);
	AppendScriptParam("View Node", SCRIPT_PARAM::BOOL, &bDisplayNode);
}

CAfterImage::~CAfterImage()
{
	Delete_Array(m_BoneArr);
}

void CAfterImage::begin()
{
	CAnimator3D* pAnimator = GetOwner()->GetParent()->Animator3D();
	int			 BoneCount = (int)pAnimator->GetBoneCount();

	for (int i = 0; i < AfterImageMaxCount; ++i)
	{
		CStructuredBuffer* pBuffer = new CStructuredBuffer;
		pBuffer->Create(sizeof(Matrix), BoneCount, SB_READ_TYPE::READ_WRITE, true);

		m_BoneArr[i] = pBuffer;
	}
}

void CAfterImage::tick()
{
	fUpdateTimer -= DT;

	if (fUpdateTimer <= 0.f)
	{
		CAnimator3D* pAnimator = nullptr;
		pAnimator			   = GetOwner()->GetParent()->Animator3D();

		fUpdateTimer = m_info.TimeStep;

		if (m_info.NodeCount > 1)
		{
			for (int i = m_info.NodeCount; i > 1; --i)
			{
				m_info.WorldTransform[i - 1] = m_info.WorldTransform[i - 2];

				if (pAnimator != nullptr)
				{
					m_info.AnimationClipIdx[i - 1] = m_info.AnimationClipIdx[i - 2];
					m_info.AnimationRatio[i - 1]   = m_info.AnimationRatio[i - 2];
				}
			}
		}

		Matrix CurrentWolrdMat = GetOwner()->GetParent()->Transform()->GetWorldMat();

		m_info.WorldTransform[0] = CurrentWolrdMat;

		if (pAnimator != nullptr)
		{
			m_info.AnimationClipIdx[0] = pAnimator->GetCurClip();
			m_info.AnimationRatio[0]   = pAnimator->GetCurClipLength();

			UpdateBoneMatrix();
		}
	}

	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();

	pMainCam->RegisterAfterImage(this->GetOwner()->GetParent(), m_info);
}

void CAfterImage::UpdateBoneMatrix()
{
	CAnimator3D* pAnimator = GetOwner()->GetParent()->Animator3D();
	int			 boneCount = pAnimator->GetBoneCount();

	// 새로운 포즈를 첫 번째 버퍼에 저장
	m_BoneArr[0]->SetData(pAnimator->GetFinalBoneMat(), 1);

	// 나머지 버퍼들을 한 칸씩 밀기
	for (int i = m_info.NodeCount - 1; i > 0; --i)
	{
		void* data = nullptr;
		if (data != nullptr)
		{
			m_BoneArr[i - 1]->GetData(&data, 1);
			m_BoneArr[i]->SetData(data, 1);
		}
	}
}

void CAfterImage::ParticularUpdateData(string _Key)
{
	if (_Key != AfterImageUpdateKey)
		return;

	CStructuredBuffer* AfterImageBuffer = CDevice::GetInst()->GetStructuredBuffer(SB_TYPE::AFTERIMAGE);
	AfterImageBuffer->SetData(&m_info, 1);
	AfterImageBuffer->UpdateData(29);

	for (int i = 0; i < m_info.NodeCount; ++i)
	{
		if (m_BoneArr[i] != nullptr)
		{
			m_BoneArr[i]->UpdateData(40 + i);
		}
	}
}

void CAfterImage::ParticularClear(string _Key)
{
	if (_Key != AfterImageUpdateKey)
		return;

	CStructuredBuffer* AfterImageBuffer = CDevice::GetInst()->GetStructuredBuffer(SB_TYPE::AFTERIMAGE);
	AfterImageBuffer->Clear(29);

	for (int i = 0; i < AfterImageMaxCount; ++i)
	{
		if (m_BoneArr[i] != nullptr)
		{
			m_BoneArr[i]->Clear(40 + i);
		}
	}
}

#define TagAfterImageCount "[AfterImage Node Count]"
#define TagAfterImageTimeStep "[After Image TimeStep]"
#define TagRenderNode "[Node bRender]"

void CAfterImage::SaveToFile(ofstream& fout)
{
	fout << TagAfterImageCount << endl;
	fout << m_info.NodeCount << endl;

	fout << TagAfterImageTimeStep << endl;
	fout << m_info.TimeStep << endl;

	fout << TagRenderNode << endl;
	fout << (int)bDisplayNode << endl;
}

void CAfterImage::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagAfterImageCount);
	fin >> m_info.NodeCount;

	Utils::GetLineUntilString(fin, TagAfterImageTimeStep);
	fin >> m_info.TimeStep;

	Utils::GetLineUntilString(fin, TagRenderNode);
	fin >> bDisplayNode;
}
