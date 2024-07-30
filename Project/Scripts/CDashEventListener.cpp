#include "pch.h"
#include "CDashEventListener.h"

void CDashEventListener::begin()
{
}

void CDashEventListener::tick()
{
}

CDashEventListener::CDashEventListener()
	: CEventListener((UINT)SCRIPT_TYPE::DASHEVENTLISTENER)
{
}

CDashEventListener::~CDashEventListener()
{
}
