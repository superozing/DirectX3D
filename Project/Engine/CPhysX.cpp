#include "pch.h"
#include "CPhysX.h"

#include "CTransform.h"
#include <DirectXMath.h>
#include "CPhysXMgr.h"
#include "CScript.h"

CPhysX::CPhysX()
	: CComponent(COMPONENT_TYPE::PHYSX)
{
}

CPhysX::~CPhysX()
{
}

void CPhysX::updateFromPhysics()
{
	if (nullptr == m_Actor)
		return;

	PxTransform PhysTrans = getTransform();
	auto		PhysPos	  = Vec3(PhysTrans.p.x, PhysTrans.p.y, PhysTrans.p.z);
	auto		FinalPos  = PhysPos - m_vOffsetPos;

	auto tempmat = XMMatrixIdentity();

	auto   ObScale	= Transform()->GetRelativeScale();
	Matrix matScale = XMMatrixScaling(ObScale.x, ObScale.y, ObScale.z);

	auto q = XMFLOAT4(PhysTrans.q.x, PhysTrans.q.y, PhysTrans.q.z, PhysTrans.q.w);

	Vec3 AxisAngle = RoRMath::QuaternionToEulerAngles(q);

	Matrix matRotX = XMMatrixRotationX(AxisAngle.y);
	Matrix matRotY = XMMatrixRotationY(AxisAngle.x);
	Matrix matRotZ = XMMatrixRotationZ(AxisAngle.z);

	Matrix matTranslation = XMMatrixTranslation(FinalPos.x, FinalPos.y, FinalPos.z);

	Transform()->m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

	// 물체의 방향값을 다시 계산한다.
	static const Vec3 arrAxis[3] = {Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f)};

	// Vec3 를 Vec4 타입으로 확장해서 행렬을 적용시켜야 함
	// XMVector3TransformCoord	- w 를 1로 확장
	// XMVector3TransformNormal - w 를 0으로 확장
	// mul(float4(_in.vPos, 1 or 0), g_matWorld);
	// 적용 받을 상태행렬의 이동을 적용할지 말지 결정
	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
		Transform()->m_arrLocalDir[i] = XMVector3TransformNormal(arrAxis[i], Transform()->m_matWorld);
		Transform()->m_arrWorldDir[i] = Transform()->m_arrLocalDir[i].Normalize();
	}

	Transform()->SetWorldMat(Transform()->m_matWorld);
	Transform()->m_matWorldInv = XMMatrixInverse(nullptr, Transform()->m_matWorld);
}

void CPhysX::updateToPhysics()
{
	if (nullptr == m_Actor)
		return;

	auto Obj = GetOwner();
	auto Rot = Obj->Transform()->GetWorldRot();
	auto Pos = Obj->Transform()->GetWorldPos() + m_vOffsetPos;

	Matrix worldMat	   = Obj->Transform()->GetWorldMat();
	Matrix transInvMat = Matrix::CreateTranslation(worldMat.Translation()).Invert();
	Matrix scaleInvMat = Matrix::CreateScale(Obj->Transform()->GetRelativeScale()).Invert();
	Matrix rotationMat = scaleInvMat * worldMat;

	Quaternion quaternion = Quaternion::CreateFromRotationMatrix(rotationMat);
	// Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(Rot.y, Rot.z, Rot.x);

	// 게임 오브젝트의 위치와 회전 정보
	PxTransform transform(PxVec3(Pos.x, Pos.y, Pos.z), PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w));

	m_Actor->setGlobalPose(transform);
}

void CPhysX::begin()
{
	CPhysXMgr::GetInst()->addGameObject(GetOwner());
}

void CPhysX::finaltick()
{
	// 콘텍트벡터를 초기화
	if (0 != m_vThisFrameContact.size())
	{
		m_vThisFrameContact.resize(0);
	}

	// imgui가 포커스되어있으면 ,현재프레임에 충돌한 오브젝트정보를 수집
	if (true == m_bImguiDirtyFlag)
	{
		m_bThisFrameImguiFocused = true;
	}
	else
	{
		m_bThisFrameImguiFocused = false;
	}
	m_bImguiDirtyFlag = false;

	if (m_bStaticActor)
	{
		updateToPhysics();
	}
	else
	{
		updateFromPhysics();
	}

	if (nullptr == m_Actor)
		return;
	const auto& trans		  = Transform();
	auto		ObjWorldPos	  = trans->GetWorldPos();
	auto		DebugFinalPos = ObjWorldPos + m_vOffsetPos;
	auto		Rot			  = getTransform().q;

	if (m_bDrawing)
	{
		if (PhysShape::BOX == m_Shape)
		{
			GamePlayStatic::DrawDebugCube(DebugFinalPos, m_vScale, Vec4(Rot.x, Rot.y, Rot.z, Rot.w),
										  Vec3(0.3f, .3f, 0.3f), false);
		}
		else
		{
			GamePlayStatic::DrawDebugSphere(DebugFinalPos, m_vScale.x / 2.f, Vec3(0.3f, .3f, 0.3f), false);
		}
	}
}

#define TagStatic "[IsStatic]"
#define TagShape "[Shape]"
#define TagScale "[Scale]"
#define TagOffsetPos "[OffsetPos]"
void CPhysX::SaveToFile(FILE* _File)
{
}

void CPhysX::SaveToFile(ofstream& fout)
{
	fout << TagStatic << endl;
	fout << m_bStaticActor << endl;

	fout << TagShape << endl;
	auto shape = magic_enum::enum_name<PhysShape>(m_Shape);
	fout << ToString(shape) << endl;

	fout << TagScale << endl;
	fout << m_vScale << endl;

	fout << TagOffsetPos << endl;
	fout << m_vOffsetPos << endl;
}

void CPhysX::LoadFromFile(FILE* _File)
{
}

void CPhysX::LoadFromFile(ifstream& fin)
{
	string tag, str;
	Utils::GetLineUntilString(fin, TagStatic);
	fin >> m_bStaticActor;

	Utils::GetLineUntilString(fin, TagShape);
	getline(fin, str);
	m_Shape = magic_enum::enum_cast<PhysShape>(str).value();

	Utils::GetLineUntilString(fin, TagScale);
	fin >> m_vScale;

	Utils::GetLineUntilString(fin, TagOffsetPos);
	fin >> m_vOffsetPos;
}

void CPhysX::setTransform(const PxTransform& transform)
{
	m_Actor->setGlobalPose(transform);
}

PxTransform CPhysX::getTransform() const
{
	return m_Actor->getGlobalPose();
}

void CPhysX::BeginOverlap(CGameObject* other)
{
	m_vThisFrameContact.push_back(tCollisionData{other, eColType::COL_BEGINE});
	++m_CollisionCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(this, other, other->PhysX());
	}
}

void CPhysX::Overlap(CGameObject* other)
{
	m_vThisFrameContact.push_back(tCollisionData{other, eColType::COL_ON});
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->Overlap(this, other, other->PhysX());
	}
}

void CPhysX::EndOverlap(CGameObject* other)
{
	--m_CollisionCount;
	m_vThisFrameContact.push_back(tCollisionData{other, eColType::COL_END});
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(this, other, other->PhysX());
	}
}
