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

	m_info.NodeCount	= 10;
	m_info.TimeStep		= 0.1f;
	m_info.fMaxLifeTime = 0.1f;
	m_info.iColorMode	= (int)ColorMode::Original;
	fUpdateTimer		= 0.1f;
	bDisplay			= true;

	fSetLifeTime = m_info.fMaxLifeTime;

	strDisplayColorMode = ToString(magic_enum::enum_name((ColorMode)m_info.iColorMode));

	m_info.AfterImageColor = Vec4(0.f, 0.f, 0.f, 0.f);

	AppendScriptParam("Node Count", SCRIPT_PARAM::INT, &m_info.NodeCount, 0, 10, false, "Afterimage Node count");
	AppendScriptParam("Time Interval", SCRIPT_PARAM::FLOAT, &m_info.TimeStep);
	AppendScriptParam("Max Life Time", SCRIPT_PARAM::FLOAT, &fSetLifeTime);

	AppendScriptParam("Color Mode", SCRIPT_PARAM::STRING, &strDisplayColorMode);
	SameLine();
	AppendMemberFunction("Change", SCRIPT_PARAM::FUNC_MEMBER, "", std::bind(&CAfterImage::ChangeColorMode, this));

	AppendScriptParam("AfterImageColor", SCRIPT_PARAM::COLOR, &m_info.AfterImageColor);

	AppendScriptParam("View Node", SCRIPT_PARAM::BOOL, &bDisplay);
}

CAfterImage::~CAfterImage()
{
	Delete_Array(m_BoneArr);
}

void CAfterImage::begin()
{
	CAnimator3D* pAnimator = GetOwner()->GetParent()->Animator3D();
	if (pAnimator != nullptr)
	{
		int BoneCount = (int)pAnimator->GetBoneCount();

		for (int i = 0; i < AfterImageMaxCount; ++i)
		{
			CStructuredBuffer* pBuffer = new CStructuredBuffer;
			pBuffer->Create(sizeof(Matrix), BoneCount, SB_READ_TYPE::READ_WRITE, true);

			m_BoneArr[i] = pBuffer;
		}
	}
	else
	{
		for (int i = 0; i < AfterImageMaxCount; ++i)
		{
			CStructuredBuffer* pBuffer = new CStructuredBuffer;
			pBuffer->Create(sizeof(Matrix), 1, SB_READ_TYPE::READ_WRITE, true);

			m_BoneArr[i] = pBuffer;
		}
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
			for (int i = 1; m_info.NodeCount > i; ++i)
			{
				m_info.WorldTransform[i] = m_info.WorldTransform[i + 1];
				m_info.fLifeTime[i]		 = m_info.fLifeTime[i + 1];
			}
		}

		Matrix CurrentWolrdMat = GetOwner()->GetParent()->Transform()->GetWorldMat();

		m_info.WorldTransform[m_info.NodeCount - 1] = CurrentWolrdMat;
		m_info.fLifeTime[m_info.NodeCount - 1]		= m_info.fMaxLifeTime;

		if (pAnimator != nullptr)
		{
			UpdateBoneMatrix();
		}
	}

	CalLifeTime(DT);

	m_info.fMaxLifeTime = fSetLifeTime;

	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();

	if (bDisplay == true)
		pMainCam->RegisterAfterImage(this->GetOwner()->GetParent(), m_info);
}

void CAfterImage::UpdateBoneMatrix() // 본은 hlsl에서 거꾸로 사용한다. 구조화 버퍼 에러
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

void CAfterImage::CalLifeTime(float _Time)
{
	for (int i = 0; i < AfterImageMaxCount; ++i)
	{
		if (m_info.fLifeTime[i] > 0.f)
			m_info.fLifeTime[i] -= _Time;
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

void CAfterImage::ChangeColorMode()
{
	if (m_info.iColorMode == 0)
	{
		m_info.iColorMode	= 1;
		strDisplayColorMode = ToString(magic_enum::enum_name((ColorMode)m_info.iColorMode));
	}
	else if (m_info.iColorMode == 1)
	{
		m_info.iColorMode	= 0;
		strDisplayColorMode = ToString(magic_enum::enum_name((ColorMode)m_info.iColorMode));
	}
}

#define TagAfterImageCount "[AfterImage Node Count]"
#define TagAfterImageTimeStep "[After Image TimeStep]"
#define TagRenderNode "[Node bRender]"
#define TagMaxLifeTime "[AfterImage Max Life Time]"
#define TagColorType "[AfterImage Color Type]"
#define TagCustomColor "[AfterImage Custom Color]"

void CAfterImage::SaveToFile(ofstream& fout)
{
	fout << TagAfterImageCount << endl;
	fout << m_info.NodeCount << endl;

	fout << TagRenderNode << endl;
	fout << (int)bDisplay << endl;

	fout << TagAfterImageTimeStep << endl;
	fout << m_info.TimeStep << endl;

	fout << TagMaxLifeTime << endl;
	fout << fSetLifeTime << endl;

	fout << TagColorType << endl;
	fout << m_info.iColorMode << endl;

	fout << TagCustomColor << endl;
	fout << m_info.AfterImageColor.x << " " << m_info.AfterImageColor.y << " " << m_info.AfterImageColor.z << " "
		 << m_info.AfterImageColor.w << endl;
}

void CAfterImage::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagAfterImageCount);
	fin >> m_info.NodeCount;

	Utils::GetLineUntilString(fin, TagRenderNode);
	fin >> bDisplay;

	Utils::GetLineUntilString(fin, TagAfterImageTimeStep);
	fin >> m_info.TimeStep;

	Utils::GetLineUntilString(fin, TagMaxLifeTime);
	fin >> m_info.fMaxLifeTime;

	fSetLifeTime = m_info.fMaxLifeTime;

	Utils::GetLineUntilString(fin, TagColorType);
	fin >> m_info.iColorMode;

	strDisplayColorMode = ToString(magic_enum::enum_name((ColorMode)m_info.iColorMode));

	Vec4 vColor;

	Utils::GetLineUntilString(fin, TagCustomColor);
	fin >> vColor.x >> vColor.y >> vColor.z >> vColor.w;

	m_info.AfterImageColor = vColor;
}
