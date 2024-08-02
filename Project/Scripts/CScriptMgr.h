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
	SPRINGARM,
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
	WRAPIMAGE,
	LEVELTRANSITION,
	EDITORCAMERAMOVESCRIPT,
	SPAWNSPOTSCRIPT,
	MEMORYPOOLMGRSCRIPT,
	HALOSCRIPT,
	STUDENTSCRIPT,
	PLAYERSCRIPT,
	DIALOG,
	PLAYERDAMAGEDSCRIPT,
	WALLSCRIPT,
	COVERLOW,
	BOSSMISSILESCRIPT,
	BOSSSCRIPT,
	DAMAGEDDIRECTION,
	MEGAFISTSCRIPT,
	ATLASIMAGEUISCRIPT,
	DAMAGEDDIRECTIONMGR,
	HUD,
	MUZZLEFLASHSCRIPT,
	BLOOMSCRIPT,
	BOSSSHIELDSCRIPT,
	BULLETMARKSPAWNER,
	BULLETSHELLSPAWNER,
	DIGITUI,
	RELOADUI,
	SHOOTINGSYSTEMSCRIPT,
	STRETCHUISCRIPT,
	TEXTUI,
	WELCOMETUTORIALEVENT,
	COVERAREA,
	COVERUI,
	DASHEVENTLISTENER,
	SHOOTINGTUTORIALEVENT,
	DIRECTIONALLIGHT,
	ARONA,
	COVERHIGHTUTORIALEVENT,
	TUTORIALGAMEMODE,
	TUTORIALTARGET,
	TITLE,
	TITLETEX,
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
