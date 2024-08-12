#include "pch.h"
#include "CScriptMgr.h"

#include "CBackgroundScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CCameraMoveScript.h"
#include "CRenderMgrScript.h"
#include "CTimeMgrScript.h"
#include "CCameraEffect.h"
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
#include "CEditorCameraMoveScript.h"
#include "CMemoryPoolMgrScript.h"
#include "CHaloScript.h"
#include "CPlayerScript.h"
#include "CDialog.h"
#include "CPlayerDamagedScript.h"
#include "CBossMissileScript.h"
#include "CBossScript.h"
#include "CDamagedDirection.h"
#include "CMegaFistScript.h"
#include "CAtlasImageUIScript.h"
#include "CDamagedDirectionMgr.h"
#include "CHUD.h"
#include "CBossShieldScript.h"
#include "CDigitUI.h"
#include "CReloadUI.h"
#include "CStretchUIScript.h"
#include "CTextUI.h"
#include "CCoverArea.h"
#include "CCoverUI.h"
#include "CArona.h"
#include "CLevelTransition.h"
#include "CBloomScript.h"
#include "CObjectGrayMaskScript.h"
#include "CSpawnSpotScript.h"
#include "CSpringArm.h"
#include "CBoostScript.h"
#include "CMissileTrailScript.h"
#include "CMuzzleFlashScript.h"
#include "CBulletScript.h"
#include "CButtons.h"
#include "CCoverHIghTutorialEvent.h"
#include "CTutorialGameMode.h"
#include "CDirectionalLight.h"
#include "CTitle.h"
#include "CWallScript.h"
#include "CWrapImage.h"
#include "CTitleTex.h"
#include "CTutorialTarget.h"
#include "CShootingSystemScript.h"
#include "CAfterImage.h"
#include "CBossBulletShellSpawner.h"
#include "CBossSmokeWaveScript.h"
#include "CChildCollider.h"
#include "CCoverJumpTutorialEvent.h"
#include "CCoverLowEventListener.h"
#include "CMiniGunScript.h"
#include "CParticleSpawnScript.h"
#include "CSmokeScript.h"
#include "CSpawnPhysX.h"
#include "CTutorialEndingEvent.h"
#include "CBossSwordTrailScript.h"
#include "CBossSwordBeamScript.h"
#include "CSBSmokeScript.h"
#include "CBulletLineScript.h"
#include "CGroundCrackScript.h"
#include "CVideoPlayer.h"
#include "CEventListener.h"
#include "CCubePushScript.h"
#include "CFinishBalloon.h"
#include "CBossLV.h"
#include "CBossOpeningTex.h"
#include "CStartLightTex.h"
#include "CStartTex.h"
#include "CVictoryBG.h"
#include "CVictoryTex.h"
#include "CDamageRay.h"
#include "CWarningBGTex.h"
#include "CWarningDownDeco.h"
#include "CWarningIcon.h"
#include "CWarningTex.h"
#include "CWarningUpDeco.h"
#include "CBossGroggy.h"
#include "CShootingSystem_DroidAR.h"
#include "CTurretShootingSystem.h"
#include "CDroidAR.h"
#include "CMuzzleFlash_DroidAR.h"
#include "CTurret.h"
#include "CTurretBulletLine.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBackgroundScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CRenderMgrScript");
	_vec.push_back(L"CTimeMgrScript");
	_vec.push_back(L"CCameraEffect");
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
	_vec.push_back(L"CEditorCameraMoveScript");
	_vec.push_back(L"CMemoryPoolMgrScript");
	_vec.push_back(L"CHaloScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CDialog");
	_vec.push_back(L"CPlayerDamagedScript");
	_vec.push_back(L"CBossMissileScript");
	_vec.push_back(L"CBossScript");
	_vec.push_back(L"CDamagedDirection");
	_vec.push_back(L"CMegaFistScript");
	_vec.push_back(L"CAtlasImageUIScript");
	_vec.push_back(L"CDamagedDirectionMgr");
	_vec.push_back(L"CHUD");
	_vec.push_back(L"CBossShieldScript");
	_vec.push_back(L"CDigitUI");
	_vec.push_back(L"CReloadUI");
	_vec.push_back(L"CStretchUIScript");
	_vec.push_back(L"CTextUI");
	_vec.push_back(L"CCoverArea");
	_vec.push_back(L"CCoverUI");
	_vec.push_back(L"CArona");
	_vec.push_back(L"CLevelTransition");
	_vec.push_back(L"CBloomScript");
	_vec.push_back(L"CObjectGrayMaskScript");
	_vec.push_back(L"CSpawnSpotScript");
	_vec.push_back(L"CSpringArm");
	_vec.push_back(L"CBoostScript");
	_vec.push_back(L"CMissileTrailScript");
	_vec.push_back(L"CMuzzleFlashScript");
	_vec.push_back(L"CBulletScript");
	_vec.push_back(L"CButtons");
	_vec.push_back(L"CCoverHIghTutorialEvent");
	_vec.push_back(L"CTutorialGameMode");
	_vec.push_back(L"CDirectionalLight");
	_vec.push_back(L"CTitle");
	_vec.push_back(L"CWallScript");
	_vec.push_back(L"CWrapImage");
	_vec.push_back(L"CTitleTex");
	_vec.push_back(L"CTutorialTarget");
	_vec.push_back(L"CShootingSystemScript");
	_vec.push_back(L"CAfterImage");
	_vec.push_back(L"CBossBulletShellSpawner");
	_vec.push_back(L"CBossSmokeWaveScript");
	_vec.push_back(L"CChildCollider");
	_vec.push_back(L"CCoverJumpTutorialEvent");
	_vec.push_back(L"CCoverLowEventListener");
	_vec.push_back(L"CMiniGunScript");
	_vec.push_back(L"CParticleSpawnScript");
	_vec.push_back(L"CSmokeScript");
	_vec.push_back(L"CSpawnPhysX");
	_vec.push_back(L"CTutorialEndingEvent");
	_vec.push_back(L"CBossSwordTrailScript");
	_vec.push_back(L"CBossSwordBeamScript");
	_vec.push_back(L"CSBSmokeScript");
	_vec.push_back(L"CBulletLineScript");
	_vec.push_back(L"CGroundCrackScript");
	_vec.push_back(L"CVideoPlayer");
	_vec.push_back(L"CEventListener");
	_vec.push_back(L"CCubePushScript");
	_vec.push_back(L"CFinishBalloon");
	_vec.push_back(L"CBossLV");
	_vec.push_back(L"CBossOpeningTex");
	_vec.push_back(L"CStartLightTex");
	_vec.push_back(L"CStartTex");
	_vec.push_back(L"CVictoryBG");
	_vec.push_back(L"CVictoryTex");
	_vec.push_back(L"CDamageRay");
	_vec.push_back(L"CWarningBGTex");
	_vec.push_back(L"CWarningDownDeco");
	_vec.push_back(L"CWarningIcon");
	_vec.push_back(L"CWarningTex");
	_vec.push_back(L"CWarningUpDeco");
	_vec.push_back(L"CBossGroggy");
	_vec.push_back(L"CShootingSystem_DroidAR");
	_vec.push_back(L"CTurretShootingSystem");
	_vec.push_back(L"CDroidAR");
	_vec.push_back(L"CMuzzleFlash_DroidAR");
	_vec.push_back(L"CTurret");
	_vec.push_back(L"CTurretBulletLine");
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
	if (L"CEditorCameraMoveScript" == _strScriptName)
		return new CEditorCameraMoveScript;
	if (L"CMemoryPoolMgrScript" == _strScriptName)
		return new CMemoryPoolMgrScript;
	if (L"CHaloScript" == _strScriptName)
		return new CHaloScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CDialog" == _strScriptName)
		return new CDialog;
	if (L"CPlayerDamagedScript" == _strScriptName)
		return new CPlayerDamagedScript;
	if (L"CBossMissileScript" == _strScriptName)
		return new CBossMissileScript;
	if (L"CBossScript" == _strScriptName)
		return new CBossScript;
	if (L"CDamagedDirection" == _strScriptName)
		return new CDamagedDirection;
	if (L"CMegaFistScript" == _strScriptName)
		return new CMegaFistScript;
	if (L"CAtlasImageUIScript" == _strScriptName)
		return new CAtlasImageUIScript;
	if (L"CDamagedDirectionMgr" == _strScriptName)
		return new CDamagedDirectionMgr;
	if (L"CHUD" == _strScriptName)
		return new CHUD;
	if (L"CBossShieldScript" == _strScriptName)
		return new CBossShieldScript;
	if (L"CDigitUI" == _strScriptName)
		return new CDigitUI;
	if (L"CReloadUI" == _strScriptName)
		return new CReloadUI;
	if (L"CStretchUIScript" == _strScriptName)
		return new CStretchUIScript;
	if (L"CTextUI" == _strScriptName)
		return new CTextUI;
	if (L"CCoverArea" == _strScriptName)
		return new CCoverArea;
	if (L"CCoverUI" == _strScriptName)
		return new CCoverUI;
	if (L"CArona" == _strScriptName)
		return new CArona;
	if (L"CLevelTransition" == _strScriptName)
		return new CLevelTransition;
	if (L"CBloomScript" == _strScriptName)
		return new CBloomScript;
	if (L"CObjectGrayMaskScript" == _strScriptName)
		return new CObjectGrayMaskScript;
	if (L"CSpawnSpotScript" == _strScriptName)
		return new CSpawnSpotScript;
	if (L"CSpringArm" == _strScriptName)
		return new CSpringArm;
	if (L"CBoostScript" == _strScriptName)
		return new CBoostScript;
	if (L"CMissileTrailScript" == _strScriptName)
		return new CMissileTrailScript;
	if (L"CMuzzleFlashScript" == _strScriptName)
		return new CMuzzleFlashScript;
	if (L"CBulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CButtons" == _strScriptName)
		return new CButtons;
	if (L"CCoverHIghTutorialEvent" == _strScriptName)
		return new CCoverHIghTutorialEvent;
	if (L"CTutorialGameMode" == _strScriptName)
		return new CTutorialGameMode;
	if (L"CDirectionalLight" == _strScriptName)
		return new CDirectionalLight;
	if (L"CTitle" == _strScriptName)
		return new CTitle;
	if (L"CWallScript" == _strScriptName)
		return new CWallScript;
	if (L"CWrapImage" == _strScriptName)
		return new CWrapImage;
	if (L"CTitleTex" == _strScriptName)
		return new CTitleTex;
	if (L"CTutorialTarget" == _strScriptName)
		return new CTutorialTarget;
	if (L"CShootingSystemScript" == _strScriptName)
		return new CShootingSystemScript;
	if (L"CAfterImage" == _strScriptName)
		return new CAfterImage;
	if (L"CBossBulletShellSpawner" == _strScriptName)
		return new CBossBulletShellSpawner;
	if (L"CBossSmokeWaveScript" == _strScriptName)
		return new CBossSmokeWaveScript;
	if (L"CChildCollider" == _strScriptName)
		return new CChildCollider;
	if (L"CCoverJumpTutorialEvent" == _strScriptName)
		return new CCoverJumpTutorialEvent;
	if (L"CCoverLowEventListener" == _strScriptName)
		return new CCoverLowEventListener;
	if (L"CMiniGunScript" == _strScriptName)
		return new CMiniGunScript;
	if (L"CParticleSpawnScript" == _strScriptName)
		return new CParticleSpawnScript;
	if (L"CSmokeScript" == _strScriptName)
		return new CSmokeScript;
	if (L"CSpawnPhysX" == _strScriptName)
		return new CSpawnPhysX;
	if (L"CTutorialEndingEvent" == _strScriptName)
		return new CTutorialEndingEvent;
	if (L"CBossSwordTrailScript" == _strScriptName)
		return new CBossSwordTrailScript;
	if (L"CBossSwordBeamScript" == _strScriptName)
		return new CBossSwordBeamScript;
	if (L"CSBSmokeScript" == _strScriptName)
		return new CSBSmokeScript;
	if (L"CBulletLineScript" == _strScriptName)
		return new CBulletLineScript;
	if (L"CGroundCrackScript" == _strScriptName)
		return new CGroundCrackScript;
	if (L"CVideoPlayer" == _strScriptName)
		return new CVideoPlayer;
	if (L"CEventListener" == _strScriptName)
		return new CEventListener;
	if (L"CCubePushScript" == _strScriptName)
		return new CCubePushScript;
	if (L"CFinishBalloon" == _strScriptName)
		return new CFinishBalloon;
	if (L"CBossLV" == _strScriptName)
		return new CBossLV;
	if (L"CBossOpeningTex" == _strScriptName)
		return new CBossOpeningTex;
	if (L"CStartLightTex" == _strScriptName)
		return new CStartLightTex;
	if (L"CStartTex" == _strScriptName)
		return new CStartTex;
	if (L"CVictoryBG" == _strScriptName)
		return new CVictoryBG;
	if (L"CVictoryTex" == _strScriptName)
		return new CVictoryTex;
	if (L"CDamageRay" == _strScriptName)
		return new CDamageRay;
	if (L"CWarningBGTex" == _strScriptName)
		return new CWarningBGTex;
	if (L"CWarningDownDeco" == _strScriptName)
		return new CWarningDownDeco;
	if (L"CWarningIcon" == _strScriptName)
		return new CWarningIcon;
	if (L"CWarningTex" == _strScriptName)
		return new CWarningTex;
	if (L"CWarningUpDeco" == _strScriptName)
		return new CWarningUpDeco;
	if (L"CBossGroggy" == _strScriptName)
		return new CBossGroggy;
	if (L"CShootingSystem_DroidAR" == _strScriptName)
		return new CShootingSystem_DroidAR;
	if (L"CTurretShootingSystem" == _strScriptName)
		return new CTurretShootingSystem;
	if (L"CDroidAR" == _strScriptName)
		return new CDroidAR;
	if (L"CMuzzleFlash_DroidAR" == _strScriptName)
		return new CMuzzleFlash_DroidAR;
	if (L"CTurret" == _strScriptName)
		return new CTurret;
	if (L"CTurretBulletLine" == _strScriptName)
		return new CTurretBulletLine;
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
	case (UINT)SCRIPT_TYPE::EDITORCAMERAMOVESCRIPT:
		return new CEditorCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::MEMORYPOOLMGRSCRIPT:
		return new CMemoryPoolMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::HALOSCRIPT:
		return new CHaloScript;
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
	case (UINT)SCRIPT_TYPE::ATLASIMAGEUISCRIPT:
		return new CAtlasImageUIScript;
		break;
	case (UINT)SCRIPT_TYPE::DAMAGEDDIRECTIONMGR:
		return new CDamagedDirectionMgr;
		break;
	case (UINT)SCRIPT_TYPE::HUD:
		return new CHUD;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSHIELDSCRIPT:
		return new CBossShieldScript;
		break;
	case (UINT)SCRIPT_TYPE::DIGITUI:
		return new CDigitUI;
		break;
	case (UINT)SCRIPT_TYPE::RELOADUI:
		return new CReloadUI;
		break;
	case (UINT)SCRIPT_TYPE::STRETCHUISCRIPT:
		return new CStretchUIScript;
		break;
	case (UINT)SCRIPT_TYPE::TEXTUI:
		return new CTextUI;
		break;
	case (UINT)SCRIPT_TYPE::COVERAREA:
		return new CCoverArea;
		break;
	case (UINT)SCRIPT_TYPE::COVERUI:
		return new CCoverUI;
		break;
	case (UINT)SCRIPT_TYPE::ARONA:
		return new CArona;
		break;
	case (UINT)SCRIPT_TYPE::LEVELTRANSITION:
		return new CLevelTransition;
		break;
	case (UINT)SCRIPT_TYPE::BLOOMSCRIPT:
		return new CBloomScript;
		break;
	case (UINT)SCRIPT_TYPE::OBJECTGRAYMASKSCRIPT:
		return new CObjectGrayMaskScript;
		break;
	case (UINT)SCRIPT_TYPE::SPAWNSPOTSCRIPT:
		return new CSpawnSpotScript;
		break;
	case (UINT)SCRIPT_TYPE::SPRINGARM:
		return new CSpringArm;
		break;
	case (UINT)SCRIPT_TYPE::BOOSTSCRIPT:
		return new CBoostScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILETRAILSCRIPT:
		return new CMissileTrailScript;
		break;
	case (UINT)SCRIPT_TYPE::MUZZLEFLASHSCRIPT:
		return new CMuzzleFlashScript;
		break;
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT:
		return new CBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::BUTTONS:
		return new CButtons;
		break;
	case (UINT)SCRIPT_TYPE::COVERHIGHTUTORIALEVENT:
		return new CCoverHIghTutorialEvent;
		break;
	case (UINT)SCRIPT_TYPE::TUTORIALGAMEMODE:
		return new CTutorialGameMode;
		break;
	case (UINT)SCRIPT_TYPE::DIRECTIONALLIGHT:
		return new CDirectionalLight;
		break;
	case (UINT)SCRIPT_TYPE::TITLE:
		return new CTitle;
		break;
	case (UINT)SCRIPT_TYPE::WALLSCRIPT:
		return new CWallScript;
		break;
	case (UINT)SCRIPT_TYPE::WRAPIMAGE:
		return new CWrapImage;
		break;
	case (UINT)SCRIPT_TYPE::TITLETEX:
		return new CTitleTex;
		break;
	case (UINT)SCRIPT_TYPE::TUTORIALTARGET:
		return new CTutorialTarget;
		break;
	case (UINT)SCRIPT_TYPE::SHOOTINGSYSTEMSCRIPT:
		return new CShootingSystemScript;
		break;
	case (UINT)SCRIPT_TYPE::AFTERIMAGE:
		return new CAfterImage;
		break;
	case (UINT)SCRIPT_TYPE::BOSSBULLETSHELLSPAWNER:
		return new CBossBulletShellSpawner;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSMOKEWAVESCRIPT:
		return new CBossSmokeWaveScript;
		break;
	case (UINT)SCRIPT_TYPE::CHILDCOLLIDER:
		return new CChildCollider;
		break;
	case (UINT)SCRIPT_TYPE::COVERJUMPTUTORIALEVENT:
		return new CCoverJumpTutorialEvent;
		break;
	case (UINT)SCRIPT_TYPE::COVERLOWEVENTLISTENER:
		return new CCoverLowEventListener;
		break;
	case (UINT)SCRIPT_TYPE::MINIGUNSCRIPT:
		return new CMiniGunScript;
		break;
	case (UINT)SCRIPT_TYPE::PARTICLESPAWNSCRIPT:
		return new CParticleSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::SMOKESCRIPT:
		return new CSmokeScript;
		break;
	case (UINT)SCRIPT_TYPE::SPAWNPHYSX:
		return new CSpawnPhysX;
		break;
	case (UINT)SCRIPT_TYPE::TUTORIALENDINGEVENT:
		return new CTutorialEndingEvent;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSWORDTRAILSCRIPT:
		return new CBossSwordTrailScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSWORDBEAMSCRIPT:
		return new CBossSwordBeamScript;
		break;
	case (UINT)SCRIPT_TYPE::SBSMOKESCRIPT:
		return new CSBSmokeScript;
		break;
	case (UINT)SCRIPT_TYPE::BULLETLINESCRIPT:
		return new CBulletLineScript;
		break;
	case (UINT)SCRIPT_TYPE::GROUNDCRACKSCRIPT:
		return new CGroundCrackScript;
		break;
	case (UINT)SCRIPT_TYPE::VIDEOPLAYER:
		return new CVideoPlayer;
		break;
	case (UINT)SCRIPT_TYPE::EVENTLISTENER:
		return new CEventListener;
		break;
	case (UINT)SCRIPT_TYPE::CUBEPUSHSCRIPT:
		return new CCubePushScript;
		break;
	case (UINT)SCRIPT_TYPE::FINISHBALLOON:
		return new CFinishBalloon;
		break;
	case (UINT)SCRIPT_TYPE::BOSSLV:
		return new CBossLV;
		break;
	case (UINT)SCRIPT_TYPE::BOSSOPENINGTEX:
		return new CBossOpeningTex;
		break;
	case (UINT)SCRIPT_TYPE::STARTLIGHTTEX:
		return new CStartLightTex;
		break;
	case (UINT)SCRIPT_TYPE::STARTTEX:
		return new CStartTex;
		break;
	case (UINT)SCRIPT_TYPE::VICTORYBG:
		return new CVictoryBG;
		break;
	case (UINT)SCRIPT_TYPE::VICTORYTEX:
		return new CVictoryTex;
		break;
	case (UINT)SCRIPT_TYPE::DAMAGERAY:
		return new CDamageRay;
		break;
	case (UINT)SCRIPT_TYPE::WARNINGBGTEX:
		return new CWarningBGTex;
		break;
	case (UINT)SCRIPT_TYPE::WARNINGDOWNDECO:
		return new CWarningDownDeco;
		break;
	case (UINT)SCRIPT_TYPE::WARNINGICON:
		return new CWarningIcon;
		break;
	case (UINT)SCRIPT_TYPE::WARNINGTEX:
		return new CWarningTex;
		break;
	case (UINT)SCRIPT_TYPE::WARNINGUPDECO:
		return new CWarningUpDeco;
		break;
	case (UINT)SCRIPT_TYPE::BOSSGROGGY:
		return new CBossGroggy;
		break;
	case (UINT)SCRIPT_TYPE::SHOOTINGSYSTEM_DROIDAR:
		return new CShootingSystem_DroidAR;
		break;
	case (UINT)SCRIPT_TYPE::TURRETSHOOTINGSYSTEM:
		return new CTurretShootingSystem;
		break;
	case (UINT)SCRIPT_TYPE::DROIDAR:
		return new CDroidAR;
		break;
	case (UINT)SCRIPT_TYPE::MUZZLEFLASH_DROIDAR:
		return new CMuzzleFlash_DroidAR;
		break;
	case (UINT)SCRIPT_TYPE::TURRET:
		return new CTurret;
		break;
	case (UINT)SCRIPT_TYPE::TURRETBULLETLINE:
		return new CTurretBulletLine;
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

	case SCRIPT_TYPE::EDITORCAMERAMOVESCRIPT:
		return L"CEditorCameraMoveScript";
		break;

	case SCRIPT_TYPE::MEMORYPOOLMGRSCRIPT:
		return L"CMemoryPoolMgrScript";
		break;

	case SCRIPT_TYPE::HALOSCRIPT:
		return L"CHaloScript";
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

	case SCRIPT_TYPE::ATLASIMAGEUISCRIPT:
		return L"CAtlasImageUIScript";
		break;

	case SCRIPT_TYPE::DAMAGEDDIRECTIONMGR:
		return L"CDamagedDirectionMgr";
		break;

	case SCRIPT_TYPE::HUD:
		return L"CHUD";
		break;

	case SCRIPT_TYPE::BOSSSHIELDSCRIPT:
		return L"CBossShieldScript";
		break;

	case SCRIPT_TYPE::DIGITUI:
		return L"CDigitUI";
		break;

	case SCRIPT_TYPE::RELOADUI:
		return L"CReloadUI";
		break;

	case SCRIPT_TYPE::STRETCHUISCRIPT:
		return L"CStretchUIScript";
		break;

	case SCRIPT_TYPE::TEXTUI:
		return L"CTextUI";
		break;

	case SCRIPT_TYPE::COVERAREA:
		return L"CCoverArea";
		break;

	case SCRIPT_TYPE::COVERUI:
		return L"CCoverUI";
		break;

	case SCRIPT_TYPE::ARONA:
		return L"CArona";
		break;

	case SCRIPT_TYPE::LEVELTRANSITION:
		return L"CLevelTransition";
		break;

	case SCRIPT_TYPE::BLOOMSCRIPT:
		return L"CBloomScript";
		break;

	case SCRIPT_TYPE::OBJECTGRAYMASKSCRIPT:
		return L"CObjectGrayMaskScript";
		break;

	case SCRIPT_TYPE::SPAWNSPOTSCRIPT:
		return L"CSpawnSpotScript";
		break;

	case SCRIPT_TYPE::SPRINGARM:
		return L"CSpringArm";
		break;

	case SCRIPT_TYPE::BOOSTSCRIPT:
		return L"CBoostScript";
		break;

	case SCRIPT_TYPE::MISSILETRAILSCRIPT:
		return L"CMissileTrailScript";
		break;

	case SCRIPT_TYPE::MUZZLEFLASHSCRIPT:
		return L"CMuzzleFlashScript";
		break;

	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"CBulletScript";
		break;

	case SCRIPT_TYPE::BUTTONS:
		return L"CButtons";
		break;

	case SCRIPT_TYPE::COVERHIGHTUTORIALEVENT:
		return L"CCoverHIghTutorialEvent";
		break;

	case SCRIPT_TYPE::TUTORIALGAMEMODE:
		return L"CTutorialGameMode";
		break;

	case SCRIPT_TYPE::DIRECTIONALLIGHT:
		return L"CDirectionalLight";
		break;

	case SCRIPT_TYPE::TITLE:
		return L"CTitle";
		break;

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"CWallScript";
		break;

	case SCRIPT_TYPE::WRAPIMAGE:
		return L"CWrapImage";
		break;

	case SCRIPT_TYPE::TITLETEX:
		return L"CTitleTex";
		break;

	case SCRIPT_TYPE::TUTORIALTARGET:
		return L"CTutorialTarget";
		break;

	case SCRIPT_TYPE::SHOOTINGSYSTEMSCRIPT:
		return L"CShootingSystemScript";
		break;

	case SCRIPT_TYPE::AFTERIMAGE:
		return L"CAfterImage";
		break;

	case SCRIPT_TYPE::BOSSBULLETSHELLSPAWNER:
		return L"CBossBulletShellSpawner";
		break;

	case SCRIPT_TYPE::BOSSSMOKEWAVESCRIPT:
		return L"CBossSmokeWaveScript";
		break;

	case SCRIPT_TYPE::CHILDCOLLIDER:
		return L"CChildCollider";
		break;

	case SCRIPT_TYPE::COVERJUMPTUTORIALEVENT:
		return L"CCoverJumpTutorialEvent";
		break;

	case SCRIPT_TYPE::COVERLOWEVENTLISTENER:
		return L"CCoverLowEventListener";
		break;

	case SCRIPT_TYPE::MINIGUNSCRIPT:
		return L"CMiniGunScript";
		break;

	case SCRIPT_TYPE::PARTICLESPAWNSCRIPT:
		return L"CParticleSpawnScript";
		break;

	case SCRIPT_TYPE::SMOKESCRIPT:
		return L"CSmokeScript";
		break;

	case SCRIPT_TYPE::SPAWNPHYSX:
		return L"CSpawnPhysX";
		break;

	case SCRIPT_TYPE::TUTORIALENDINGEVENT:
		return L"CTutorialEndingEvent";
		break;

	case SCRIPT_TYPE::BOSSSWORDTRAILSCRIPT:
		return L"CBossSwordTrailScript";
		break;

	case SCRIPT_TYPE::BOSSSWORDBEAMSCRIPT:
		return L"CBossSwordBeamScript";
		break;

	case SCRIPT_TYPE::SBSMOKESCRIPT:
		return L"CSBSmokeScript";
		break;

	case SCRIPT_TYPE::BULLETLINESCRIPT:
		return L"CBulletLineScript";
		break;

	case SCRIPT_TYPE::GROUNDCRACKSCRIPT:
		return L"CGroundCrackScript";
		break;

	case SCRIPT_TYPE::VIDEOPLAYER:
		return L"CVideoPlayer";
		break;

	case SCRIPT_TYPE::EVENTLISTENER:
		return L"CEventListener";
		break;

	case SCRIPT_TYPE::CUBEPUSHSCRIPT:
		return L"CCubePushScript";
		break;

	case SCRIPT_TYPE::FINISHBALLOON:
		return L"CFinishBalloon";
		break;

	case SCRIPT_TYPE::BOSSLV:
		return L"CBossLV";
		break;

	case SCRIPT_TYPE::BOSSOPENINGTEX:
		return L"CBossOpeningTex";
		break;

	case SCRIPT_TYPE::STARTLIGHTTEX:
		return L"CStartLightTex";
		break;

	case SCRIPT_TYPE::STARTTEX:
		return L"CStartTex";
		break;

	case SCRIPT_TYPE::VICTORYBG:
		return L"CVictoryBG";
		break;

	case SCRIPT_TYPE::VICTORYTEX:
		return L"CVictoryTex";
		break;

	case SCRIPT_TYPE::DAMAGERAY:
		return L"CDamageRay";
		break;

	case SCRIPT_TYPE::WARNINGBGTEX:
		return L"CWarningBGTex";
		break;

	case SCRIPT_TYPE::WARNINGDOWNDECO:
		return L"CWarningDownDeco";
		break;

	case SCRIPT_TYPE::WARNINGICON:
		return L"CWarningIcon";
		break;

	case SCRIPT_TYPE::WARNINGTEX:
		return L"CWarningTex";
		break;

	case SCRIPT_TYPE::WARNINGUPDECO:
		return L"CWarningUpDeco";
		break;

	case SCRIPT_TYPE::BOSSGROGGY:
		return L"CBossGroggy";
		break;

	case SCRIPT_TYPE::SHOOTINGSYSTEM_DROIDAR:
		return L"CShootingSystem_DroidAR";
		break;

	case SCRIPT_TYPE::TURRETSHOOTINGSYSTEM:
		return L"CTurretShootingSystem";
		break;

	case SCRIPT_TYPE::DROIDAR:
		return L"CDroidAR";
		break;

	case SCRIPT_TYPE::MUZZLEFLASH_DROIDAR:
		return L"CMuzzleFlash_DroidAR";
		break;

	case SCRIPT_TYPE::TURRET:
		return L"CTurret";
		break;

	case SCRIPT_TYPE::TURRETBULLETLINE:
		return L"CTurretBulletLine";
		break;

	}
	return nullptr;
}