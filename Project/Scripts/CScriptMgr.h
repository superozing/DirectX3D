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
	WEAPONINFO,
	PAUSEPANEL,
	PAUSEBTN,
	DAMAGEFONT,
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
