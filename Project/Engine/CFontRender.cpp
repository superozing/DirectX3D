#include "pch.h"
#include "CFontRender.h"

#include "CTransform.h"

#include "CFontMgr.h"
#include "CDevice.h"


CFontRender::CFontRender()
	: CRenderComponent(COMPONENT_TYPE::FONTRENDER)
	, m_FontInfo{}
{
}

CFontRender::~CFontRender()
{
}
void CFontRender::finaltick()
{

}

void CFontRender::render()
{
	Transform()->UpdateData();

	Vec4 vWorldPos;
	if (GetOwner()->GetLayerIdx() != 31)
		//vWorldPos = Vector4::Transform(Vec4(Transform()->GetWorldPos(), 1), g_Transform.matWV);
		vWorldPos = g_Transform.matWVP.Translation();

	m_FontInfo.vPos = ConvertUIWorldPosToWindowPos(vWorldPos);

	if (m_FontInfo.WStr.size() && m_FontInfo.fFontSize)
		CFontMgr::GetInst()->DrawFont(m_FontInfo);
}

Vec3 CFontRender::ConvertUIWorldPosToWindowPos(Vec3 _WorldPos)
{
	if (!m_AllowConvertWorldPosToWindowPos)
		return _WorldPos;

	Vec3 vPos = _WorldPos;
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	vPos.x += vResolution.x / 2.f ;
	vPos.y = -vPos.y + vResolution.y / 2.f;
	
	vPos.x += m_FontOffset.x;
	vPos.y += m_FontOffset.y;

	return vPos;
}

void CFontRender::SaveToFile(FILE* _File)
{
}

void CFontRender::LoadFromFile(FILE* _File)
{
}
