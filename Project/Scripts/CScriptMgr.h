#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	BACKGROUNDSCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	CAMERAMOVESCRIPT,
	RENDERMGRSCRIPT,
	TIMEMGRSCRIPT,
	CAMERAEFFECT,
	BTNUISCRIPT,
	IMAGEUISCRIPT,
	PANELUISCRIPT,
	CROSSHAIR,
	PAUSEPANEL,
	PAUSEBTN,
	PROGRESSBAR,
	BOSSHP,
	MONSTERHP,
	PLAYERHP,
	AMMOINFO,
	PHYSXMGRSCRIPT,
	SKILLICON,
	EDITORCAMERAMOVESCRIPT,
	MEMORYPOOLMGRSCRIPT,
	HALOSCRIPT,
	STUDENTSCRIPT,
	PLAYERSCRIPT,
	DIALOG,
	PLAYERDAMAGEDSCRIPT,
	BOSSMISSILESCRIPT,
	BOSSSCRIPT,
	DAMAGEDDIRECTION,
	MEGAFISTSCRIPT,
	ATLASIMAGEUISCRIPT,
	DAMAGEDDIRECTIONMGR,
	HUD,
	BOSSSHIELDSCRIPT,
	DIGITUI,
	RELOADUI,
	STRETCHUISCRIPT,
	TEXTUI,
	WELCOMETUTORIALEVENT,
	COVERAREA,
	COVERUI,
	DASHEVENTLISTENER,
	SHOOTINGTUTORIALEVENT,
	ARONA,
	LEVELTRANSITION,
	BLOOMSCRIPT,
	OBJECTGRAYMASKSCRIPT,
	SPAWNSPOTSCRIPT,
	SPRINGARM,
	BOOSTSCRIPT,
	MISSILETRAILSCRIPT,
	MUZZLEFLASHSCRIPT,
	BULLETSCRIPT,
	BUTTONS,
	COVERHIGHTUTORIALEVENT,
	TUTORIALGAMEMODE,
	DIRECTIONALLIGHT,
	TITLE,
	WALLSCRIPT,
	WRAPIMAGE,
	TITLETEX,
	TUTORIALTARGET,
	SHOOTINGSYSTEMSCRIPT,
	AFTERIMAGE,
	BULLETLINESCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public: 
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript* GetScript(const wstring& _strScriptName);
	static CScript* GetScript(UINT _iScriptType);
	static const wchar_t* GetScriptName(CScript* _pScript);
};
