#include "pch.h"
#include "CFrustum.h"

#include "CCamera.h"

CFrustum::CFrustum()
    : m_Owner(nullptr)
    , m_ProjPos{}
    , m_arrFace{}
{
	// 투영좌표계 초기 좌표 설정
    //     4 -- 5
    //   / |   /|
    //  /  7 -/ 6
    // 0 -- 1  /
    // | /  | /
    // 3 -- 2

    m_ProjPos[0] = Vec3(-1.f, 1.f, 0.f);
    m_ProjPos[1] = Vec3(1.f, 1.f, 0.f);
    m_ProjPos[2] = Vec3(1.f, -1.f, 0.f);
    m_ProjPos[3] = Vec3(-1.f, -1.f, 0.f);

    m_ProjPos[4] = Vec3(-1.f, 1.f, 1.f);
    m_ProjPos[5] = Vec3(1.f, 1.f, 1.f);
    m_ProjPos[6] = Vec3(1.f, -1.f, 1.f);
    m_ProjPos[7] = Vec3(-1.f, -1.f, 1.f);

}

CFrustum::~CFrustum()
{
}


void CFrustum::finaltick()
{
    Matrix matInv = m_Owner->GetProjInvMat() * m_Owner->GetViewInvMat();

    Vec3 vWorld[8] = {};
    for (int i = 0; i < 8; ++i)
    {
        vWorld[i] = XMVector3TransformCoord(m_ProjPos[i], matInv);
    }

	// WorldSpace 상에서 카메라의 시야를 표현하는 6 개의 평면 제작
	//     4 -- 5
	//   / |   /|
	//  /  7 -/ 6
	// 0 -- 1  /
	// | /  | /
	// 3 -- 2
	m_arrFace[(UINT)FACE::F_NEAR] = XMPlaneFromPoints(vWorld[0], vWorld[1], vWorld[2]);
	m_arrFace[(UINT)FACE::F_FAR] = XMPlaneFromPoints(vWorld[5], vWorld[4], vWorld[7]);
	m_arrFace[(UINT)FACE::F_TOP] = XMPlaneFromPoints(vWorld[0], vWorld[4], vWorld[5]);
	m_arrFace[(UINT)FACE::F_BOT] = XMPlaneFromPoints(vWorld[2], vWorld[6], vWorld[7]);
	m_arrFace[(UINT)FACE::F_LEFT] = XMPlaneFromPoints(vWorld[0], vWorld[7], vWorld[4]);
	m_arrFace[(UINT)FACE::F_RIGHT] = XMPlaneFromPoints(vWorld[1], vWorld[5], vWorld[6]);
}

bool CFrustum::FrustumCheck(Vec3 _vWorldPos)
{
	// ->
	// N dot P + D > 0
	for (int i = 0; i < 6; ++i)
	{
		float dot = _vWorldPos.Dot(Vec3(m_arrFace[i].x, m_arrFace[i].y, m_arrFace[i].z));

		if (dot + m_arrFace[i].w > 0)
		{
			return false;
		}
	}

	return true;
}

bool CFrustum::FrustumCheck(Vec3 _Center, float _Radius)
{
	// ->
	// N dot P + D > 0
	for (int i = 0; i < 6; ++i)
	{
		float dot = _Center.Dot(Vec3(m_arrFace[i].x, m_arrFace[i].y, m_arrFace[i].z));

		if (dot + m_arrFace[i].w > _Radius)
		{
			return false;
		}
	}

	return true;
}
