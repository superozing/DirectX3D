#pragma once

#include <Engine\CScript.h>

typedef void (*SetObjSpawnType)(int);

enum class SpawnObjType
{
	Player,
	Monster,
	BOSS,
	ETC,
	NONE,
	END
};

class CSpawnSpotScript : public CScript
{
private:
	Vec3   SpawnBasicPosition;
	Vec3   SpawnBasicScale;
	string SpawnObjectPath;

	SpawnObjType SpawnObjType;
	int			 iTypeMask;

public:
	void SetSpawnObjType(int iNewType);

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

public:
	CLONE_DISABLE(CSpawnSpotScript);
	CSpawnSpotScript();
	~CSpawnSpotScript();
};
