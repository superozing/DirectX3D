#pragma once
#include <Engine\singleton.h>

class CGameObjectEx;

class CEditorObjMgr : public CManager<CEditorObjMgr>
{
	SINGLE(CEditorObjMgr);

private:
	vector<CGameObjectEx*> m_vecEditorObj;

	CGameObjectEx* m_EditorCam;

public:
	virtual void init() override;
	virtual void enter() override {}

	void progress();

	CGameObjectEx*				 GetEditorCam() { return m_EditorCam; }
	const vector<CGameObjectEx*> GetEditorGameObjects() { return m_vecEditorObj; }
};
