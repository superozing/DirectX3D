#include "pch.h"
#include "CDialog.h"

CDialog::CDialog()
	: CScript((UINT)SCRIPT_TYPE::DIALOG)
{
}

CDialog::~CDialog()
{
}

void CDialog::begin()
{
}

void CDialog::tick()
{
}

void CDialog::SaveToFile(FILE* _File)
{
}

void CDialog::SaveToFile(ofstream& fout)
{
}

void CDialog::LoadFromFile(FILE* _File)
{
}

void CDialog::LoadFromFile(ifstream& fin)
{
}
