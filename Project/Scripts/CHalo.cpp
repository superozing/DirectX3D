#include "pch.h"
#include "CHalo.h"

CHalo::CHalo()
	: CScript((UINT)SCRIPT_TYPE::HALO)
{
}

CHalo::~CHalo()
{
}

void CHalo::begin()
{
}

#include "CSpringArm.h"
#include <Engine/CLogMgr.h>

void CHalo::tick()
{
	auto pSA = GetOwner()->GetScript<CSpringArm>();

	if (!pSA || !Animator3D())
		return;

	const vector<tMTBone>* pBones = Animator3D()->GetBones();

	int idx = -1;
	for (size_t i = 0; i < pBones->size(); i++)
	{
		wstring strBoneName = (*pBones)[i].strBoneName;
		if (strBoneName == L"Bip001 Head")
		{
			idx = i;
			break;
		}
	}

	int iClipIdx = Animator3D()->GetCurClip();
	int iFrmIdx	 = Animator3D()->GetCurFrameIdx();

	if (idx == -1)
	{
		wstring msg;
		msg += GetOwner()->GetName() + L"'s Head Bones is Not Found";
		CLogMgr::GetInst()->AddLog(Log_Level::WARN, msg);

		return;
	}

	tMTKeyFrame keyFrame = (*pBones)[idx].vecKeyFrame[iClipIdx][iFrmIdx];

	Matrix matTrans = Matrix::CreateTranslation(keyFrame.vTranslate);
	Matrix matScale = Matrix::CreateScale(keyFrame.vScale);
	Matrix matRot	= Matrix::CreateFromQuaternion(keyFrame.qRot);

	Matrix matFinal = matScale * matRot * matTrans * Transform()->GetWorldMat();

	GamePlayStatic::DrawDebugSphere(matFinal.Translation(), 20.f, Vec3(0.7f, 0.7f, 0.3f), false);

	pSA->SetOffsetPos(matFinal.Translation() - Transform()->GetWorldPos());
	pSA->SetDistance(150.f);

	Vec3 vFront = -matFinal.Right();

	pSA->SetDir(vFront.Normalize());
}
