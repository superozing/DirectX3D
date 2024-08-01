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
#include "CPausePanel.h"
#include "CPauseBtn.h"
#include "CProgressBar.h"
#include "CBossHP.h"
#include "CMonsterHP.h"
#include "CPlayerHP.h"
#include "CAmmoInfo.h"
#include "CPhysXMgrScript.h"
#include "CSkillIcon.h"
#include "CWrapImage.h"
#include "CLevelTransition.h"
#include "CEditorCameraMoveScript.h"
#include "CSpawnSpotScript.h"
#include "CMemoryPoolMgrScript.h"
#include "CHaloScript.h"
#include "CStudentScript.h"
#include "CPlayerScript.h"
#include "CDialog.h"
#include "CPlayerDamagedScript.h"
#include "CWallScript.h"
#include "CCoverLow.h"
#include "CBossMissileScript.h"
#include "CBossScript.h"
#include "CDamagedDirection.h"
#include "CMegaFistScript.h"
#include "CTutorialGameMode.h"
#include "CAtlasImageUIScript.h"
#include "CDamagedDirectionMgr.h"
#include "CHUD.h"
#include "CMuzzleFlashScript.h"
#include "CBloomScript.h"
#include "CBossShieldScript.h"
#include "CBulletMarkSpawner.h"
#include "CBulletShellSpawner.h"
#include "CDigitUI.h"
#include "CReloadUI.h"
#include "CShootingSystemScript.h"
#include "CStretchUIScript.h"
#include "CTextUI.h"
#include "CWelcomeTutorialEvent.h"
#include "CCoverArea.h"
#include "CCoverUI.h"
#include "CDashEventListener.h"
#include "CShootingTutorialEvent.h"
#include "CDirectionalLight.h"
#include "CArona.h"
#include "CTutorialTarget.h"
#include "CCoverHIghTutorialEvent.h"

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
	_vec.push_back(L"CPausePanel");
	_vec.push_back(L"CPauseBtn");
	_vec.push_back(L"CProgressBar");
	_vec.push_back(L"CBossHP");
	_vec.push_back(L"CMonsterHP");
	_vec.push_back(L"CPlayerHP");
	_vec.push_back(L"CAmmoInfo");
	_vec.push_back(L"CPhysXMgrScript");
	_vec.push_back(L"CSkillIcon");
	_vec.push_back(L"CWrapImage");
	_vec.push_back(L"CLevelTransition");
	_vec.push_back(L"CEditorCameraMoveScript");
	_vec.push_back(L"CSpawnSpotScript");
	_vec.push_back(L"CMemoryPoolMgrScript");
	_vec.push_back(L"CHaloScript");
	_vec.push_back(L"CStudentScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CDialog");
	_vec.push_back(L"CPlayerDamagedScript");
	_vec.push_back(L"CWallScript");
	_vec.push_back(L"CCoverLow");
	_vec.push_back(L"CBossMissileScript");
	_vec.push_back(L"CBossScript");
	_vec.push_back(L"CDamagedDirection");
	_vec.push_back(L"CMegaFistScript");
	_vec.push_back(L"CTutorialGameMode");
	_vec.push_back(L"CAtlasImageUIScript");
	_vec.push_back(L"CDamagedDirectionMgr");
	_vec.push_back(L"CHUD");
	_vec.push_back(L"CMuzzleFlashScript");
	_vec.push_back(L"CBloomScript");
	_vec.push_back(L"CBossShieldScript");
	_vec.push_back(L"CBulletMarkSpawner");
	_vec.push_back(L"CBulletShellSpawner");
	_vec.push_back(L"CDigitUI");
	_vec.push_back(L"CReloadUI");
	_vec.push_back(L"CShootingSystemScript");
	_vec.push_back(L"CStretchUIScript");
	_vec.push_back(L"CTextUI");
	_vec.push_back(L"CWelcomeTutorialEvent");
	_vec.push_back(L"CCoverArea");
	_vec.push_back(L"CCoverUI");
	_vec.push_back(L"CDashEventListener");
	_vec.push_back(L"CShootingTutorialEvent");
	_vec.push_back(L"CDirectionalLight");
	_vec.push_back(L"CArona");
	_vec.push_back(L"CTutorialTarget");
	_vec.push_back(L"CCoverHIghTutorialEvent");
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
	if (L"CPausePanel" == _strScriptName)
		return new CPausePanel;
	if (L"CPauseBtn" == _strScriptName)
		return new CPauseBtn;
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
	if (L"CLevelTransition" == _strScriptName)
		return new CLevelTransition;
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
	if (L"CWallScript" == _strScriptName)
		return new CWallScript;
	if (L"CCoverLow" == _strScriptName)
		return new CCoverLow;
	if (L"CBossMissileScript" == _strScriptName)
		return new CBossMissileScript;
	if (L"CBossScript" == _strScriptName)
		return new CBossScript;
	if (L"CDamagedDirection" == _strScriptName)
		return new CDamagedDirection;
	if (L"CMegaFistScript" == _strScriptName)
		return new CMegaFistScript;
	if (L"CTutorialGameMode" == _strScriptName)
		return new CTutorialGameMode;
	if (L"CAtlasImageUIScript" == _strScriptName)
		return new CAtlasImageUIScript;
	if (L"CDamagedDirectionMgr" == _strScriptName)
		return new CDamagedDirectionMgr;
	if (L"CHUD" == _strScriptName)
		return new CHUD;
	if (L"CMuzzleFlashScript" == _strScriptName)
		return new CMuzzleFlashScript;
	if (L"CBloomScript" == _strScriptName)
		return new CBloomScript;
	if (L"CBossShieldScript" == _strScriptName)
		return new CBossShieldScript;
	if (L"CBulletMarkSpawner" == _strScriptName)
		return new CBulletMarkSpawner;
	if (L"CBulletShellSpawner" == _strScriptName)
		return new CBulletShellSpawner;
	if (L"CDigitUI" == _strScriptName)
		return new CDigitUI;
	if (L"CReloadUI" == _strScriptName)
		return new CReloadUI;
	if (L"CShootingSystemScript" == _strScriptName)
		return new CShootingSystemScript;
	if (L"CStretchUIScript" == _strScriptName)
		return new CStretchUIScript;
	if (L"CTextUI" == _strScriptName)
		return new CTextUI;
	if (L"CWelcomeTutorialEvent" == _strScriptName)
		return new CWelcomeTutorialEvent;
	if (L"CCoverArea" == _strScriptName)
		return new CCoverArea;
	if (L"CCoverUI" == _strScriptName)
		return new CCoverUI;
	if (L"CDashEventListener" == _strScriptName)
		return new CDashEventListener;
	if (L"CShootingTutorialEvent" == _strScriptName)
		return new CShootingTutorialEvent;
	if (L"CDirectionalLight" == _strScriptName)
		return new CDirectionalLight;
	if (L"CArona" == _strScriptName)
		return new CArona;
	if (L"CTutorialTarget" == _strScriptName)
		return new CTutorialTarget;
	if (L"CCoverHIghTutorialEvent" == _strScriptName)
		return new CCoverHIghTutorialEvent;
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
	case (UINT)SCRIPT_TYPE::PAUSEPANEL:
		return new CPausePanel;
		break;
	case (UINT)SCRIPT_TYPE::PAUSEBTN:
		return new CPauseBtn;
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
	case (UINT)SCRIPT_TYPE::LEVELTRANSITION:
		return new CLevelTransition;
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
	case (UINT)SCRIPT_TYPE::WALLSCRIPT:
		return new CWallScript;
		break;
	case (UINT)SCRIPT_TYPE::COVERLOW:
		return new CCoverLow;
		break;
	case (UINT)SCRIPT_TYPE::BOSSMISSILESCRIPT:
		return new CBossMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSCRIPT:
		return new CBossScript;
		break;
	case (UINT)SCRIPT_TYPE::DAMAGEDDIRECTION:
		return new CDamagedDirection;
		break;
	case (UINT)SCRIPT_TYPE::MEGAFISTSCRIPT:
		return new CMegaFistScript;
		break;
	case (UINT)SCRIPT_TYPE::TUTORIALGAMEMODE:
		return new CTutorialGameMode;
		break;
	case (UINT)SCRIPT_TYPE::ATLASIMAGEUISCRIPT:
		return new CAtlasImageUIScript;
		break;
	case (UINT)SCRIPT_TYPE::DAMAGEDDIRECTIONMGR:
		return new CDamagedDirectionMgr;
		break;
	case (UINT)SCRIPT_TYPE::HUD:
		return new CHUD;
		break;
	case (UINT)SCRIPT_TYPE::MUZZLEFLASHSCRIPT:
		return new CMuzzleFlashScript;
		break;
	case (UINT)SCRIPT_TYPE::BLOOMSCRIPT:
		return new CBloomScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSHIELDSCRIPT:
		return new CBossShieldScript;
		break;
	case (UINT)SCRIPT_TYPE::BULLETMARKSPAWNER:
		return new CBulletMarkSpawner;
		break;
	case (UINT)SCRIPT_TYPE::BULLETSHELLSPAWNER:
		return new CBulletShellSpawner;
		break;
	case (UINT)SCRIPT_TYPE::DIGITUI:
		return new CDigitUI;
		break;
	case (UINT)SCRIPT_TYPE::RELOADUI:
		return new CReloadUI;
		break;
	case (UINT)SCRIPT_TYPE::SHOOTINGSYSTEMSCRIPT:
		return new CShootingSystemScript;
		break;
	case (UINT)SCRIPT_TYPE::STRETCHUISCRIPT:
		return new CStretchUIScript;
		break;
	case (UINT)SCRIPT_TYPE::TEXTUI:
		return new CTextUI;
		break;
	case (UINT)SCRIPT_TYPE::WELCOMETUTORIALEVENT:
		return new CWelcomeTutorialEvent;
		break;
	case (UINT)SCRIPT_TYPE::COVERAREA:
		return new CCoverArea;
		break;
	case (UINT)SCRIPT_TYPE::COVERUI:
		return new CCoverUI;
		break;
	case (UINT)SCRIPT_TYPE::DASHEVENTLISTENER:
		return new CDashEventListener;
		break;
	case (UINT)SCRIPT_TYPE::SHOOTINGTUTORIALEVENT:
		return new CShootingTutorialEvent;
		break;
	case (UINT)SCRIPT_TYPE::DIRECTIONALLIGHT:
		return new CDirectionalLight;
		break;
	case (UINT)SCRIPT_TYPE::ARONA:
		return new CArona;
		break;
	case (UINT)SCRIPT_TYPE::TUTORIALTARGET:
		return new CTutorialTarget;
		break;
	case (UINT)SCRIPT_TYPE::COVERHIGHTUTORIALEVENT:
		return new CCoverHIghTutorialEvent;
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

	case SCRIPT_TYPE::PAUSEPANEL:
		return L"CPausePanel";
		break;

	case SCRIPT_TYPE::PAUSEBTN:
		return L"CPauseBtn";
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

	case SCRIPT_TYPE::LEVELTRANSITION:
		return L"CLevelTransition";
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

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"CWallScript";
		break;

	case SCRIPT_TYPE::COVERLOW:
		return L"CCoverLow";
		break;

	case SCRIPT_TYPE::BOSSMISSILESCRIPT:
		return L"CBossMissileScript";
		break;

	case SCRIPT_TYPE::BOSSSCRIPT:
		return L"CBossScript";
		break;

	case SCRIPT_TYPE::DAMAGEDDIRECTION:
		return L"CDamagedDirection";
		break;

	case SCRIPT_TYPE::MEGAFISTSCRIPT:
		return L"CMegaFistScript";
		break;

	case SCRIPT_TYPE::TUTORIALGAMEMODE:
		return L"CTutorialGameMode";
		break;

	case SCRIPT_TYPE::ATLASIMAGEUISCRIPT:
		return L"CAtlasImageUIScript";
		break;

	case SCRIPT_TYPE::DAMAGEDDIRECTIONMGR:
		return L"CDamagedDirectionMgr";
		break;

	case SCRIPT_TYPE::HUD:
		return L"CHUD";
		break;

	case SCRIPT_TYPE::MUZZLEFLASHSCRIPT:
		return L"CMuzzleFlashScript";
		break;

	case SCRIPT_TYPE::BLOOMSCRIPT:
		return L"CBloomScript";
		break;

	case SCRIPT_TYPE::BOSSSHIELDSCRIPT:
		return L"CBossShieldScript";
		break;

	case SCRIPT_TYPE::BULLETMARKSPAWNER:
		return L"CBulletMarkSpawner";
		break;

	case SCRIPT_TYPE::BULLETSHELLSPAWNER:
		return L"CBulletShellSpawner";
		break;

	case SCRIPT_TYPE::DIGITUI:
		return L"CDigitUI";
		break;

	case SCRIPT_TYPE::RELOADUI:
		return L"CReloadUI";
		break;

	case SCRIPT_TYPE::SHOOTINGSYSTEMSCRIPT:
		return L"CShootingSystemScript";
		break;

	case SCRIPT_TYPE::STRETCHUISCRIPT:
		return L"CStretchUIScript";
		break;

	case SCRIPT_TYPE::TEXTUI:
		return L"CTextUI";
		break;

	case SCRIPT_TYPE::WELCOMETUTORIALEVENT:
		return L"CWelcomeTutorialEvent";
		break;

	case SCRIPT_TYPE::COVERAREA:
		return L"CCoverArea";
		break;

	case SCRIPT_TYPE::COVERUI:
		return L"CCoverUI";
		break;

	case SCRIPT_TYPE::DASHEVENTLISTENER:
		return L"CDashEventListener";
		break;

	case SCRIPT_TYPE::SHOOTINGTUTORIALEVENT:
		return L"CShootingTutorialEvent";
		break;

	case SCRIPT_TYPE::DIRECTIONALLIGHT:
		return L"CDirectionalLight";
		break;

	case SCRIPT_TYPE::ARONA:
		return L"CArona";
		break;

	case SCRIPT_TYPE::TUTORIALTARGET:
		return L"CTutorialTarget";
		break;

	case SCRIPT_TYPE::COVERHIGHTUTORIALEVENT:
		return L"CCoverHIghTutorialEvent";
		break;

	}
	return nullptr;
}