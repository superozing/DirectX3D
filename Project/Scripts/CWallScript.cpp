#include "pch.h"
#include "CWallScript.h"

static string msg = "";
CWallScript::CWallScript()
	: CScript((UINT)SCRIPT_TYPE::WALLSCRIPT)
{
	msg = "This Is Nothing Yet.";
	AppendScriptParam("Msg", SCRIPT_PARAM::STRING, &msg);
}

CWallScript::~CWallScript()
{
}

void CWallScript::begin()
{
}

void CWallScript::tick()
{
}