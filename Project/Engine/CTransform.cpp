#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTimeMgr.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(true)
	, m_IsDynamic(true)
{
}

CTransform::~CTransform()
{
}

void CTransform::Lerp(Vec3 _pos, bool _bMoveRot, Vec3 _rot, bool _bMoveScale, Vec3 _scale, float _time)
{
	m_vTargetPos = _pos;
	_bMoveRot ? m_vTargetRot = _rot : m_vTargetRot = m_vRelativeRotation;
	_bMoveScale ? m_vTargetScale = _scale : m_vTargetScale = m_vRelativeScale;
	m_fTargetTimer = m_fTargetTime = _time;
	m_bRotLerp = _bMoveRot;
	m_bScaleLerp = _bMoveScale;
	m_bLerp = true;

	m_vStartPos = m_vRelativePos;
	m_vStartRot = m_vRelativeRotation;
	m_vStartScale = m_vRelativeScale;

	if (m_fTargetTime == 0.f) {
		m_bLerp = false;
		SetRelativePos(m_vTargetPos);
		SetRelativeRotation(m_vTargetRot);
		SetRelativeScale(m_vTargetScale);
	}
}

void CTransform::tick()
{
	// LerpToTarget;
	if (m_bLerp) 
	{
		// 목표에 도착
		Vec3 vNPos, vNRot, vNScale;
		if (m_fTargetTimer <= 0.f) {
			m_bLerp = false;

			vNPos = m_vTargetPos;
			vNRot = m_vTargetRot;
			vNScale = m_vTargetScale;
		}
		else {
			float alpha = m_fTargetTimer / m_fTargetTime;
			vNPos = RoRMath::Lerp(m_vTargetPos, m_vStartPos, alpha);
			vNRot = RoRMath::Lerp(m_vTargetRot, m_vStartRot, alpha);
			vNScale = RoRMath::Lerp(m_vTargetScale, m_vStartScale, alpha);

			m_fTargetTimer -= DT_ENGINE;
		}

		SetRelativePos(vNPos);
		if(m_bRotLerp) SetRelativePos(vNPos);
		if(m_bScaleLerp) SetRelativePos(vNPos);
	}
}

void CTransform::finaltick()
{	
	CalWorldMat();

	// 역행렬 구하기
	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::UpdateData()
{
	g_Transform.matWorld = m_matWorld;
	g_Transform.matWV = g_Transform.matWorld * g_Transform.matView;
	g_Transform.matWVP = g_Transform.matWV * g_Transform.matProj;
		
	// 위치정보를 Transform 상수버퍼에 보내고, B0 레지스터에 바인딩 해둠
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_Transform);
	pCB->UpdateData();
}

void CTransform::SetWorldMat(const Matrix& _matWorld)
{
	m_matWorld = _matWorld;
	Vec3 vScale, vRot, vPos;
	Quaternion Quat;
	Matrix matrix = m_matWorld;

	if (GetOwner()->GetParent()) 
	{
		const Matrix& matParentWorldInv = GetOwner()->GetParent()->Transform()->GetWorldInvMat();
		if (m_bAbsolute)
		{
			// m_matWorld = m_matWorld * matParentScaleInv * matParentWorld + matParentWorldInv * matParentScale;
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();
			Matrix matParentScale = XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z);

			matrix = matrix * matParentWorldInv * matParentScale;
		}
		else {
			matrix *= matParentWorldInv;
		}
	}

	matrix.Decompose(vScale, Quat, vPos);
	auto mat = XMMatrixRotationQuaternion(Quat);
	vRot = DecomposeRotMat(mat);

	SetRelativePos(vPos);
	SetRelativeRotation(vRot);
	SetRelativeScale(vScale);
}

Vec3 CTransform::GetWorldScale()
{
	if (m_bAbsolute)
	{
		return m_vRelativeScale;
	}

	CGameObject* pParent = GetOwner()->GetParent();
	Vec3 vWorldScale = m_vRelativeScale;

	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();
		pParent = pParent->GetParent();
	}

	return vWorldScale;
}

Vec3 CTransform::GetWorldRot()
{
	XMVECTOR Qurt;
	XMVECTOR Scale;
	XMVECTOR Pos;
	XMMatrixDecompose(&Scale, &Qurt, &Pos, m_matWorld);
	Matrix matRot = XMMatrixRotationQuaternion(Qurt);
	return DecomposeRotMat(matRot);
}

void CTransform::SetDir(Vec3 _Dir)
{
	_Dir.Normalize();
	Vec3 vRight = Vec3(0.f, 1.f, 0.f).Cross(_Dir);
	if (vRight.x == 0.f && vRight.y == 0.f && vRight.z == 0.f)
		vRight = Vec3(1.f, 0.f, 0.f);

	vRight.Normalize();
	Vec3 vUp = _Dir.Cross(vRight);

	Matrix matRot = XMMatrixIdentity();

	matRot._11 = vRight.x; matRot._12 = vRight.y; matRot._13 = vRight.z;
	matRot._21 = vUp.x;	   matRot._22 = vUp.y;    matRot._23 = vUp.z;
	matRot._31 = _Dir.x;   matRot._32 = _Dir.y;   matRot._33 = _Dir.z;

	Vec3 vRot = DecomposeRotMat(matRot);
	SetRelativeRotation(vRot);
}

void CTransform::SaveToFile(FILE* _File)
{
	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);	
}

#define TagPos "[Pos]"
#define TagScale "[Scale]"
#define TagRotation "[Rotation]"
#define TagAbsolute "[Absolute]"

void CTransform::SaveToFile(ofstream& fout)
{
	fout << TagPos << endl;
	fout << m_vRelativePos << endl;
	fout << TagScale << endl;
	fout << m_vRelativeScale << endl;
	fout << TagRotation << endl;
	fout << m_vRelativeRotation << endl;
	fout << TagAbsolute << endl;
	fout << m_bAbsolute << endl;
}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
}

void CTransform::LoadFromFile(ifstream& fin)
{
	string tag;

	Utils::GetLineUntilString(fin, TagPos);
	fin >> m_vRelativePos;
	
	Utils::GetLineUntilString(fin, TagScale);
	fin >> m_vRelativeScale;

	Utils::GetLineUntilString(fin, TagRotation);
	fin >> m_vRelativeRotation;

	Utils::GetLineUntilString(fin, TagAbsolute);
	fin >> m_bAbsolute;
}

void CTransform::CalWorldMat()
{
	m_matWorld = XMMatrixIdentity();

	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix matRotX = XMMatrixRotationX(m_vRelativeRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vRelativeRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRelativeRotation.z);

	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

	// 물체의 방향값을 다시 계산한다.
	static const Vec3 arrAxis[3] =
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	// Vec3 를 Vec4 타입으로 확장해서 행렬을 적용시켜야 함
	// XMVector3TransformCoord	- w 를 1로 확장
	// XMVector3TransformNormal - w 를 0으로 확장
	// mul(float4(_in.vPos, 1 or 0), g_matWorld); 
	// 적용 받을 상태행렬의 이동을 적용할지 말지 결정
	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
		m_arrLocalDir[i] = XMVector3TransformNormal(arrAxis[i], m_matWorld);
		m_arrWorldDir[i] = m_arrLocalDir[i].Normalize();
	}

	// 부모 오브젝트가 있다면
	if (GetOwner()->GetParent())
	{
		const Matrix& matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_bAbsolute)
		{
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();

			Matrix matParentScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);

			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorld *= matParentWorld;
		}

		for (int i = 0; i < 3; ++i)
		{
			// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
			m_arrWorldDir[i] = XMVector3TransformNormal(arrAxis[i], m_matWorld);
			m_arrWorldDir[i].Normalize();
		}
	}

	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}
