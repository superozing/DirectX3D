#include "pch.h"
#include "CScriptMgr.h"

#include "CBackgroundScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CRenderMgrScript.h"
#include "CTimeMgrScript.h"
#include "CCameraEffect.h"
#include "CSpringArm.h"
#include "CBtnUIScript.h"
#include "CImageUIScript.h"
#include "CPanelUIScript.h"
#include "CProgressBar.h"
#include "CPhysXMgrScript.h"
#include "CHalo.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBackgroundScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CRenderMgrScript");
	_vec.push_back(L"CTimeMgrScript");
	_vec.push_back(L"CCameraEffect");
	_vec.push_back(L"CSpringArm");
	_vec.push_back(L"CBtnUIScript");
	_vec.push_back(L"CImageUIScript");
	_vec.push_back(L"CPanelUIScript");
	_vec.push_back(L"CProgressBar");
	_vec.push_back(L"CPhysXMgrScript");
	_vec.push_back(L"CHalo");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBackgroundScript" == _strScriptName)
		return new CBackgroundScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CRenderMgrScript" == _strScriptName)
		return new CRenderMgrScript;
	if (L"CTimeMgrScript" == _strScriptName)
		return new CTimeMgrScript;
	if (L"CCameraEffect" == _strScriptName)
		return new CCameraEffect;
	if (L"CSpringArm" == _strScriptName)
		return new CSpringArm;
	if (L"CBtnUIScript" == _strScriptName)
		return new CBtnUIScript;
	if (L"CImageUIScript" == _strScriptName)
		return new CImageUIScript;
	if (L"CPanelUIScript" == _strScriptName)
		return new CPanelUIScript;
	if (L"CProgressBar" == _strScriptName)
		return new CProgressBar;
	if (L"CPhysXMgrScript" == _strScriptName)
		return new CPhysXMgrScript;
	if (L"CHalo" == _strScriptName)
		return new CHalo;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return new CBackgroundScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::RENDERMGRSCRIPT:
		return new CRenderMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::TIMEMGRSCRIPT:
		return new CTimeMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAEFFECT:
		return new CCameraEffect;
		break;
	case (UINT)SCRIPT_TYPE::SPRINGARM:
		return new CSpringArm;
		break;
	case (UINT)SCRIPT_TYPE::BTNUISCRIPT:
		return new CBtnUIScript;
		break;
	case (UINT)SCRIPT_TYPE::IMAGEUISCRIPT:
		return new CImageUIScript;
		break;
	case (UINT)SCRIPT_TYPE::PANELUISCRIPT:
		return new CPanelUIScript;
		break;
	case (UINT)SCRIPT_TYPE::PROGRESSBAR:
		return new CProgressBar;
		break;
	case (UINT)SCRIPT_TYPE::PHYSXMGRSCRIPT:
		return new CPhysXMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::HALO:
		return new CHalo;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return L"CBackgroundScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::RENDERMGRSCRIPT:
		return L"CRenderMgrScript";
		break;

	case SCRIPT_TYPE::TIMEMGRSCRIPT:
		return L"CTimeMgrScript";
		break;

	case SCRIPT_TYPE::CAMERAEFFECT:
		return L"CCameraEffect";
		break;

	case SCRIPT_TYPE::SPRINGARM:
		return L"CSpringArm";
		break;

	case SCRIPT_TYPE::BTNUISCRIPT:
		return L"CBtnUIScript";
		break;

	case SCRIPT_TYPE::IMAGEUISCRIPT:
		return L"CImageUIScript";
		break;

	case SCRIPT_TYPE::PANELUISCRIPT:
		return L"CPanelUIScript";
		break;

	case SCRIPT_TYPE::PROGRESSBAR:
		return L"CProgressBar";
		break;

	case SCRIPT_TYPE::PHYSXMGRSCRIPT:
		return L"CPhysXMgrScript";
		break;

	case SCRIPT_TYPE::HALO:
		return L"CHalo";
		break;

	}
	return nullptr;
}