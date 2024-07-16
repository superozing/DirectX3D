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
	SpawnObjType   SpawnType;
	string		   strDisplaySpawnTypeString;
	vector<string> m_vecDisplayMode;
	int			   m_ivecModeIdx;
	Vec4		   ModeColor;

	list<CGameObject*> m_listSpawnObject;
	list<CGameObject*> m_CurrentSpawnObject;

	vector<Ptr<CPrefab>> m_vecPrefab;
	vector<string>		 m_vecPrefabKey;
	CPrefab*			 m_CurBindPrefab;
	int					 m_ivecPrefabIdx;

public:
	void SetDisplayMode();
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
	void SpawnObject();
	void DeAllocateAllObject();

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(ofstream& fout) override;
	virtual void LoadFromFile(ifstream& fin) override;

public:
	CLONE(CSpawnSpotScript);
	CSpawnSpotScript();
	CSpawnSpotScript(const CSpawnSpotScript& _Origin);
	~CSpawnSpotScript();
};
