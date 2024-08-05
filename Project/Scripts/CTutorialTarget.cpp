#include "pch.h"
#include "CTutorialTarget.h"

CTutorialTarget::CTutorialTarget()
	: CScript((UINT)SCRIPT_TYPE::TUTORIALTARGET)
{
}

CTutorialTarget::~CTutorialTarget()
{
}

void CTutorialTarget::begin()
{
	Hit(false);
}

void CTutorialTarget::tick()
{
}