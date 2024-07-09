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
#include "CCrosshair.h"
#include "CWeaponInfo.h"
#include "CPausePanel.h"
#include "CPauseBtn.h"
#include "CDamageFont.h"
#include "CProgressBar.h"
#include "CBossHP.h"
#include "CMonsterHP.h"
#include "CPlayerHP.h"
#include "CAmmoInfo.h"
#include "CPhysXMgrScript.h"

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
	_vec.push_back(L"CCrosshair");
	_vec.push_back(L"CWeaponInfo");
	_vec.push_back(L"CPausePanel");
	_vec.push_back(L"CPauseBtn");
	_vec.push_back(L"CDamageFont");
	_vec.push_back(L"CProgressBar");
	_vec.push_back(L"CBossHP");
	_vec.push_back(L"CMonsterHP");
	_vec.push_back(L"CPlayerHP");
	_vec.push_back(L"CAmmoInfo");
	_vec.push_back(L"CPhysXMgrScript");
	_vec.push_back(L"CHalo");
	_vec.push_back(L"CSkillIcon");
}

CScript* CScriptMgr::GetScript(const wstring& _strScriptName)
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
	if (L"CCrosshair" == _strScriptName)
		return new CCrosshair;
	if (L"CWeaponInfo" == _strScriptName)
		return new CWeaponInfo;
	if (L"CPausePanel" == _strScriptName)
		return new CPausePanel;
	if (L"CPauseBtn" == _strScriptName)
		return new CPauseBtn;
	if (L"CDamageFont" == _strScriptName)
		return new CDamageFont;
	if (L"CProgressBar" == _strScriptName)
		return new CProgressBar;
	if (L"CBossHP" == _strScriptName)
		return new CBossHP;
	if (L"CMonsterHP" == _strScriptName)
		return new CMonsterHP;
	if (L"CPlayerHP" == _strScriptName)
		return new CPlayerHP;
	if (L"CAmmoInfo" == _strScriptName)
		return new CAmmoInfo;
	if (L"CPhysXMgrScript" == _strScriptName)
		return new CPhysXMgrScript;
	if (L"CHalo" == _strScriptName)
		return new CHalo;
	if (L"CSkillIcon" == _strScriptName)
		return new CSkillIcon;
	return nullptr;
}

CScript* CScriptMgr::GetScript(UINT _iScriptType)
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
	case (UINT)SCRIPT_TYPE::CROSSHAIR:
		return new CCrosshair;
		break;
	case (UINT)SCRIPT_TYPE::WEAPONINFO:
		return new CWeaponInfo;
		break;
	case (UINT)SCRIPT_TYPE::PAUSEPANEL:
		return new CPausePanel;
		break;
	case (UINT)SCRIPT_TYPE::PAUSEBTN:
		return new CPauseBtn;
		break;
	case (UINT)SCRIPT_TYPE::DAMAGEFONT:
		return new CDamageFont;
		break;
	case (UINT)SCRIPT_TYPE::PROGRESSBAR:
		return new CProgressBar;
		break;
	case (UINT)SCRIPT_TYPE::BOSSHP:
		return new CBossHP;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERHP:
		return new CMonsterHP;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERHP:
		return new CPlayerHP;
		break;
	case (UINT)SCRIPT_TYPE::AMMOINFO:
		return new CAmmoInfo;
		break;
	case (UINT)SCRIPT_TYPE::PHYSXMGRSCRIPT:
		return new CPhysXMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::HALO:
		return new CHalo;
		break;
	case (UINT)SCRIPT_TYPE::SKILLICON:
		return new CSkillIcon;
		break;
	}
	return nullptr;
}

const wchar_t* CScriptMgr::GetScriptName(CScript* _pScript)
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

	case SCRIPT_TYPE::CROSSHAIR:
		return L"CCrosshair";
		break;

	case SCRIPT_TYPE::WEAPONINFO:
		return L"CWeaponInfo";
		break;

	case SCRIPT_TYPE::PAUSEPANEL:
		return L"CPausePanel";
		break;

	case SCRIPT_TYPE::PAUSEBTN:
		return L"CPauseBtn";
		break;

	case SCRIPT_TYPE::DAMAGEFONT:
		return L"CDamageFont";
		break;

	case SCRIPT_TYPE::PROGRESSBAR:
		return L"CProgressBar";
		break;

	case SCRIPT_TYPE::BOSSHP:
		return L"CBossHP";
		break;

	case SCRIPT_TYPE::MONSTERHP:
		return L"CMonsterHP";
		break;

	case SCRIPT_TYPE::PLAYERHP:
		return L"CPlayerHP";
		break;

	case SCRIPT_TYPE::AMMOINFO:
		return L"CAmmoInfo";
		break;

	case SCRIPT_TYPE::PHYSXMGRSCRIPT:
		return L"CPhysXMgrScript";
		break;

	case SCRIPT_TYPE::HALO:
		return L"CHalo";
		break;

	case SCRIPT_TYPE::SKILLICON:
		return L"CSkillIcon";
		break;

	}
	return nullptr;
}