#pragma once
#include <Engine\CScript.h>

class CStudentScript : public CScript
{
public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CStudentScript);
	CStudentScript();
	~CStudentScript();
};
