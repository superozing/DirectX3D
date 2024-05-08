#pragma once
#include <Engine\singleton.h>

class CGameObjectEx;

class CEditorObjMgr :
    public CSingleton<CEditorObjMgr>
{
    SINGLE(CEditorObjMgr);
private:
    vector<CGameObjectEx*>    m_vecEditorObj;

    CGameObjectEx*            m_EditorCam;

public:
    void init();
    void progress();    
    
    CGameObjectEx* GetEditorCam() { return m_EditorCam; }
};

