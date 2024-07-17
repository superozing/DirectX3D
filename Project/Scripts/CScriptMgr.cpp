#include "pch.h"
#include "CScriptMgr.h"

#include "CBackgroundScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
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
#include "CSkillIcon.h"
#include "CWrapImage.h"
#include "CEditorCameraMoveScript.h"
#include "CSpawnSpotScript.h"
#include "CMemoryPoolMgrScript.h"
#include "CHaloScript.h"
#include "CStudentScript.h"
#include "CPlayerScript.h"
#include "CDialog.h"
#include "CPlayerDamagedScript.h"
#include "CEventDetector.h"
#include "CLevelTransition.h"
#include "CTutorialGameMode.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBackgroundScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
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
	_vec.push_back(L"CSkillIcon");
	_vec.push_back(L"CWrapImage");
	_vec.push_back(L"CEditorCameraMoveScript");
	_vec.push_back(L"CSpawnSpotScript");
	_vec.push_back(L"CMemoryPoolMgrScript");
	_vec.push_back(L"CHaloScript");
	_vec.push_back(L"CStudentScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CDialog");
	_vec.push_back(L"CPlayerDamagedScript");
	_vec.push_back(L"CEventDetector");
	_vec.push_back(L"CLevelTransition");
	_vec.push_back(L"CTutorialGameMode");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBackgroundScript" == _strScriptName)
		return new CBackgroundScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
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
	if (L"CSkillIcon" == _strScriptName)
		return new CSkillIcon;
	if (L"CWrapImage" == _strScriptName)
		return new CWrapImage;
	if (L"CEditorCameraMoveScript" == _strScriptName)
		return new CEditorCameraMoveScript;
	if (L"CSpawnSpotScript" == _strScriptName)
		return new CSpawnSpotScript;
	if (L"CMemoryPoolMgrScript" == _strScriptName)
		return new CMemoryPoolMgrScript;
	if (L"CHaloScript" == _strScriptName)
		return new CHaloScript;
	if (L"CStudentScript" == _strScriptName)
		return new CStudentScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CDialog" == _strScriptName)
		return new CDialog;
	if (L"CPlayerDamagedScript" == _strScriptName)
		return new CPlayerDamagedScript;
	if (L"CEventDetector" == _strScriptName)
		return new CEventDetector;
	if (L"CLevelTransition" == _strScriptName)
		return new CLevelTransition;
	if (L"CTutorialGameMode" == _strScriptName)
		return new CTutorialGameMode;
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
	case (UINT)SCRIPT_TYPE::SKILLICON:
		return new CSkillIcon;
		break;
	case (UINT)SCRIPT_TYPE::WRAPIMAGE:
		return new CWrapImage;
		break;
	case (UINT)SCRIPT_TYPE::EDITORCAMERAMOVESCRIPT:
		return new CEditorCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::SPAWNSPOTSCRIPT:
		return new CSpawnSpotScript;
		break;
	case (UINT)SCRIPT_TYPE::MEMORYPOOLMGRSCRIPT:
		return new CMemoryPoolMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::HALOSCRIPT:
		return new CHaloScript;
		break;
	case (UINT)SCRIPT_TYPE::STUDENTSCRIPT:
		return new CStudentScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::DIALOG:
		return new CDialog;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERDAMAGEDSCRIPT:
		return new CPlayerDamagedScript;
		break;
	case (UINT)SCRIPT_TYPE::EVENTDETECTOR:
		return new CEventDetector;
		break;
	case (UINT)SCRIPT_TYPE::LEVELTRANSITION:
		return new CLevelTransition;
		break;
	case (UINT)SCRIPT_TYPE::TUTORIALGAMEMODE:
		return new CTutorialGameMode;
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

	case SCRIPT_TYPE::SKILLICON:
		return L"CSkillIcon";
		break;

	case SCRIPT_TYPE::WRAPIMAGE:
		return L"CWrapImage";
		break;

	case SCRIPT_TYPE::EDITORCAMERAMOVESCRIPT:
		return L"CEditorCameraMoveScript";
		break;

	case SCRIPT_TYPE::SPAWNSPOTSCRIPT:
		return L"CSpawnSpotScript";
		break;

	case SCRIPT_TYPE::MEMORYPOOLMGRSCRIPT:
		return L"CMemoryPoolMgrScript";
		break;

	case SCRIPT_TYPE::HALOSCRIPT:
		return L"CHaloScript";
		break;

	case SCRIPT_TYPE::STUDENTSCRIPT:
		return L"CStudentScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::DIALOG:
		return L"CDialog";
		break;

	case SCRIPT_TYPE::PLAYERDAMAGEDSCRIPT:
		return L"CPlayerDamagedScript";
		break;

	case SCRIPT_TYPE::EVENTDETECTOR:
		return L"CEventDetector";
		break;

	case SCRIPT_TYPE::LEVELTRANSITION:
		return L"CLevelTransition";
		break;

	case SCRIPT_TYPE::TUTORIALGAMEMODE:
		return L"CTutorialGameMode";
		break;

	}
	return nullptr;
}