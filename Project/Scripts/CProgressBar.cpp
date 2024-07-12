#include "pch.h"
#include "CProgressBar.h"

#include "CPanelUIScript.h"

CProgressBar::CProgressBar()
	: CScript((UINT)SCRIPT_TYPE::PROGRESSBAR)
{
}

CProgressBar::CProgressBar(UINT _ScriptType)
	: CScript(_ScriptType)
{
}

CProgressBar::~CProgressBar()
{
}

void CProgressBar::begin()
{
	AppendScriptParam("Cur Value", SCRIPT_PARAM::INT, &m_CurValue);
	AppendScriptParam("Max Value", SCRIPT_PARAM::INT, &m_MaxValue);
	m_pPanelUI = new CPanelUIScript;

	m_pPanelUI->DisableMouseInput();
	m_pPanelUI->DisallowDragAndDrop();
	m_pPanelUI->AllowTexSet();

	GetOwner()->AddComponent(m_pPanelUI);

	MeshRender()->GetDynamicMaterial(0);

	MakeChildObjects();
}

#define TagMaxValue "[Max Value]"

void CProgressBar::SaveToFile(FILE* _File)
{
}

void CProgressBar::SaveToFile(ofstream& fout)
{
	fout << TagMaxValue << endl;
	fout << m_MaxValue << endl;
}

void CProgressBar::LoadFromFile(FILE* _File)
{
}

void CProgressBar::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagMaxValue);
	fin >> m_MaxValue;
}
