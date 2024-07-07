#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	BACKGROUNDSCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	PLAYERSCRIPT,
	CAMERAMOVESCRIPT,
	RENDERMGRSCRIPT,
	TIMEMGRSCRIPT,
	CAMERAEFFECT,
	SPRINGARM,
	BTNUISCRIPT,
	IMAGEUISCRIPT,
	PANELUISCRIPT,
	PROGRESSBAR,
	PHYSXMGRSCRIPT,
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
