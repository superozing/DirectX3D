#pragma once

#include <Engine\CScript.h>

typedef void (*SetObjSpawnType)(int);

enum class SpawnObjType
{
	None,
	Player,
	Monster,
	BOSS,
	Etc,
	END
};

class CSpawnSpotScript : public CScript
{
private:
	Vec3   SpawnBasicPosition;
	Vec3   SpawnBasicScale;
	string SpawnObjectPath;

	SpawnObjType SpawnObjType;
	string		 strDisplayString;
	Vec4		 ModeColor;

	CGameObject*		 CurObjectPointer;
	vector<CGameObject*> m_vecSpawnObject;

public:
	void SetSpawnTypePlayer();
	void SetSpawnTypeMonster();
	void SetSpawnTypeBoss();
	void SetSpawnTypeETC();
	void SetSpawnTypeNone();

	void RegisterObject();
	void SpawnObject();

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
