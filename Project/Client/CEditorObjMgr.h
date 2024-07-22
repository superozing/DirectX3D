#pragma once
#include <Engine\singleton.h>
#include <Engine/CGameObject.h>

class CGameObjectEx;

class CEditorObjMgr : public CManager<CEditorObjMgr>
{
	SINGLE(CEditorObjMgr);

private:
	vector<CGameObjectEx*> m_vecEditorObj;

	CGameObjectEx* m_EditorCam;
	CGameObjectEx* m_ManagerObj;
	CGameObjectEx* m_MemoryPool;

public:
	virtual void init() override;
	virtual void enter() override {}

	void begin();
	void progress();

	CGameObjectEx*				 GetEditorCam() { return m_EditorCam; }
	const vector<CGameObjectEx*> GetEditorGameObjects() { return m_vecEditorObj; }

	void SetEditorCam(CGameObject* _obj);

	CGameObjectEx* GetMemoryPoolObj() { return m_MemoryPool; }
};
