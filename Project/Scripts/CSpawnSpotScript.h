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

	vector<Ptr<CPrefab>> m_vecPrefab;
	vector<string>		 m_vecPrefabKey;
	int					 m_ivecSize;
	int					 m_ivecCurrentIdx;
	CPrefab*			 m_CurBindPrefab;

public:
	void SetSpawnTypePlayer();
	void SetSpawnTypeMonster();
	void SetSpawnTypeBoss();
	void SetSpawnTypeETC();
	void SetSpawnTypeNone();

	void RegisterPrefab();
	void PushBackPrefab();

	void ClearPrefab();
	void DeletePrefab();

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
