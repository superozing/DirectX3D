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
	Vec3 SpawnBasicPosition;

	SpawnObjType SpawnType;
	string		 strDisplaySpawnTypeString;
	Vec4		 ModeColor;

	int				   m_iBindObjCount;
	list<CGameObject*> m_listSpawnObject;
	list<CGameObject*> m_CurrentSpawnObject;

	vector<string> m_vecPrefabKey;

public:
	void SetSpawnTypePlayer();
	void SetSpawnTypeMonster();
	void SetSpawnTypeBoss();
	void SetSpawnTypeETC();
	void SetSpawnTypeNone();

	void RegisterObject();
	void DeAllocateObject();
	void SpawnObject();

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE_DISABLE(CSpawnSpotScript);
	CSpawnSpotScript();
	~CSpawnSpotScript();
};
