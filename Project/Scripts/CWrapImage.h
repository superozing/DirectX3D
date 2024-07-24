#pragma once
#include <Engine\CScript.h>

enum class WrapMode
{
	BasedOnTexture,
	Custom,
};

class CWrapImage : public CScript
{
private:
	WrapMode m_Mode;
	Vec2	 PlaneScale;

	Vec2 OriginTexSize;
	Vec2 WrapSize;

	Vec2 ToSaveWrapSize;

private:
	void ChangeWrapMode();

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout);
	virtual void LoadFromFile(ifstream& fin);

public:
	CLONE(CWrapImage);
	CWrapImage();
	~CWrapImage();
};
