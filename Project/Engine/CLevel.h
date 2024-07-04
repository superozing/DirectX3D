﻿#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel : public CEntity
{
private:
	CLayer*		m_arrLayer[(UINT)LAYER::LAYER_MAX];
	LEVEL_STATE m_State;
	string		m_strRelativePath;

public:
	void begin();
	void tick();
	void finaltick();

	void AddObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove = true);
	void AddObject(CGameObject* _Object, const wstring& _strLayerName, bool _bChildMove = true);

	CLayer* GetLayer(int _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }
	CLayer* GetLayer(const wstring& _strLayerName);

	CGameObject* FindObjectByName(const wstring& _strName);
	void		 FindObjectsByName(const wstring& _strName, vector<CGameObject*>& _vecObj);

	void CheckObjectName(CGameObject* _Object);

public:
	void		ChangeState(LEVEL_STATE _NextState);
	LEVEL_STATE GetState() { return m_State; }

	void   SetRelativePath(const string& _str) { m_strRelativePath = _str; }
	string GetRelativePath() { return m_strRelativePath; }

private:
	void clear();

	CLONE(CLevel);

public:
	CLevel();
	CLevel(const CLevel& _OriginLevel);
	~CLevel();

	friend class CLevelMgr;
};