#include "pch.h"
#include "PathMgr.h"

#include "function.h"
vector<wstring> g_vecName;

int main()
{
	CPathMgr::init();

	// 스크립트 매니저 생성
	ScriptNameInput();
	MakeScriptMgrHeader();
	MakeScriptMgrCPP();

	GetAllContents();

	InitStrHeader();
	MakeStrHeader("Project\\Engine\\strLevel.h", "LEVEL", g_vecLevelNames);
	MakeStrHeader("Project\\Engine\\strFx.h", "FX", g_vecFxNames);
	MakeStrHeader("Project\\Engine\\strSnd.h", "SND", g_vecSndNames);
	MakeStrHeader("Project\\Engine\\strTex.h", "TEX", g_vecTexNames);
	MakeStrHeader("Project\\Engine\\strMtrl.h", "MTRL", g_vecMtrlNames);
	MakeStrHeader("Project\\Engine\\strPref.h", "PREF", g_vecPrefNames);
	MakeStrHeader("Project\\Engine\\strAnim.h", "ANIM", g_vecAnimNames);
	MakeStrHeader("Project\\Engine\\strTxt.h", "TXT", g_vecTxtNames);
	MakeStrHeader("Project\\Engine\\strGrpShader.h", "GRPSHADER", g_vecGraphicsShaderNames);
	MakeStrHeader("Project\\Engine\\strFBX.h", "FBX", g_vecFBXNames);

	PrintError();

	return 0;
}