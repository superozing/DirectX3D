#pragma once
#include <Engine\singleton.h>
#include <Engine/CGameObject.h>

class CGameObjectEx;

enum class EDITOROBJ_TYPE
{
	EditorCam,
	Manager,
	MemoryPool,
	END
};

class CEditorObjMgr : public CManager<CEditorObjMgr>
{
	SINGLE(CEditorObjMgr);

private:
	CGameObjectEx* m_arrEditorObj[(UINT)EDITOROBJ_TYPE::END];

	CGameObjectEx* m_EditorCam;
	CGameObjectEx* m_ManagerObj;
	CGameObjectEx* m_MemoryPool;

public:
	virtual void init() override;
	virtual void enter() override {}

	void begin();
	void progress();

	CGameObjectEx*	GetEditorCam() { return m_EditorCam; }
	CGameObjectEx** GetEditorGameObjects() { return m_arrEditorObj; }

	void SetEditorCam(CGameObject* _obj);

	CGameObjectEx* GetMemoryPoolObj() { return m_MemoryPool; }
};
