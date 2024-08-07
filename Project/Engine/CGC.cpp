#include "pch.h"
#include "CGC.h"

#include "CEntity.h"

CGC::CGC()
{
}

CGC::~CGC()
{
	Delete_Vec(m_vecEntity);
}

void CGC::tick()
{
	if (50 < m_vecEntity.size())
	{
		Delete_Vec(m_vecEntity);
	}
}

void CGC::exit()
{
	Delete_Vec(m_vecEntity);
}
