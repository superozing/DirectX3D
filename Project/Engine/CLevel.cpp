﻿#include "pch.h"
#include "CLevel.h"

#include "CLevelMgr.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CTimeMgr.h"
#include "CRenderMgr.h"

CLevel::CLevel()
	: m_arrLayer{}
	, m_State(LEVEL_STATE::NONE)
	, m_isActiveUIModal(false)
{
	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		m_arrLayer[i]			   = new CLayer;
		m_arrLayer[i]->m_iLayerIdx = i;
		auto s					   = magic_enum::enum_name((LAYER)i);
		m_arrLayer[i]->SetName(ToWString(s));
	}
}

CLevel::CLevel(const CLevel& _OriginLevel)
	: CEntity(_OriginLevel)
	, m_arrLayer{}
	, m_isActiveUIModal(false)
{
	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		m_arrLayer[i] = _OriginLevel.m_arrLayer[i]->Clone();
	}
}

CLevel::~CLevel()
{
	Delete_Array(m_arrLayer);
}

void CLevel::begin()
{
	for (int i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		m_arrLayer[i]->begin();
	}
}

void CLevel::tick()
{
	if (GetModalState())
	{
		m_arrLayer[(UINT)LAYER::LAYER_UI]->tick();
		return;
	}

	for (int i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		m_arrLayer[i]->tick();
	}
}

void CLevel::finaltick()
{
	for (int i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		m_arrLayer[i]->finaltick();
	}
}

void CLevel::AddObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove)
{
	CheckObjectName(_Object);

	m_arrLayer[_LayerIdx]->AddObject(_Object, _bChildMove);
}

void CLevel::AddObject(CGameObject* _Object, const wstring& _strLayerName, bool _bChildMove)
{
	CLayer* pLayer = GetLayer(_strLayerName);
	if (nullptr == pLayer)
		return;

	CheckObjectName(_Object);

	pLayer->AddObject(_Object, _bChildMove);
}

CLayer* CLevel::GetLayer(int _iLayerIdx)
{
	if (_iLayerIdx != -1)
		return m_arrLayer[_iLayerIdx];

	return nullptr;
}

CLayer* CLevel::GetLayer(const wstring& _strLayerName)
{
	for (int i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		if (_strLayerName == m_arrLayer[i]->GetName())
		{
			return m_arrLayer[i];
		}
	}
	return nullptr;
}

CGameObject* CLevel::FindObjectByName(const wstring& _strName)
{
	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		const vector<CGameObject*>& vecParent = m_arrLayer[i]->GetParentObjects();

		for (size_t j = 0; j < vecParent.size(); ++j)
		{
			list<CGameObject*> queue;
			queue.push_back(vecParent[j]);

			// 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
			while (!queue.empty())
			{
				CGameObject* pObject = queue.front();
				queue.pop_front();

				const vector<CGameObject*>& vecChild = pObject->GetChild();
				for (size_t k = 0; k < vecChild.size(); ++k)
				{
					queue.push_back(vecChild[k]);
				}

				if (_strName == pObject->GetName())
				{
					return pObject;
				}
			}
		}
	}

	return nullptr;
}

void CLevel::FindObjectsByName(const wstring& _strName, vector<CGameObject*>& _vecObj)
{
	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		const vector<CGameObject*>& vecParent = m_arrLayer[i]->GetParentObjects();

		for (size_t j = 0; j < vecParent.size(); ++j)
		{
			list<CGameObject*> queue;
			queue.push_back(vecParent[j]);

			// 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
			while (!queue.empty())
			{
				CGameObject* pObject = queue.front();
				queue.pop_front();

				const vector<CGameObject*>& vecChild = pObject->GetChild();
				for (size_t k = 0; k < vecChild.size(); ++k)
				{
					queue.push_back(vecChild[k]);
				}

				if (_strName == pObject->GetName())
				{
					_vecObj.push_back(pObject);
				}
			}
		}
	}
}

void CLevel::CheckObjectName(CGameObject* _Object)
{
	if (nullptr != _Object)
	{
		wstring ObjName = _Object->GetName();

		CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

		if (nullptr != pLevel)
		{
			CGameObject* pObj = pLevel->FindObjectByName(ObjName);

			// 이미 동일한 이름의 오브젝트가 존재할 경우
			if (nullptr != pObj)
			{
				wchar_t szObj[255] = {};
				int		num		   = 0;

				if (nullptr != pObj)
				{
					while (true)
					{
						swprintf_s(szObj, (ObjName + L"_copy%d").c_str(), num);
						if (nullptr == pLevel->FindObjectByName(szObj))
							break;
						++num;
					}
				}

				_Object->SetName(szObj);
			}
		}

		list<CGameObject*> queue;
		queue.push_back(_Object);

		while (!queue.empty())
		{
			CGameObject* pObject = queue.front();
			queue.pop_front();

			for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
			{
				CheckObjectName(pObject->m_vecChild[i]);
				queue.push_back(pObject->m_vecChild[i]);
			}
		}
	}
}

void CLevel::ChangeLayer(CGameObject* _Object, int _LayerIdx, bool _bChildMove)
{
	m_arrLayer[_LayerIdx]->AddObject(_Object, _bChildMove);
}

void CLevel::clear()
{
	for (UINT i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
	{
		m_arrLayer[i]->m_vecObjects.clear();
	}
}

void CLevel::ChangeState(LEVEL_STATE _NextState)
{
	if (m_State == _NextState)
		return;

	// 정지 -> 플레이
	if ((LEVEL_STATE::STOP == m_State || LEVEL_STATE::PAUSE == m_State || LEVEL_STATE::NONE == m_State) &&
		LEVEL_STATE::PLAY == _NextState)
	{
		CTimeMgr::GetInst()->LockDeltaTime(false);

		// 레벨 카메라 모드
		CRenderMgr::GetInst()->ActiveEditorMode(false);

		// None, Stop -> Play
		if (LEVEL_STATE::STOP == m_State || LEVEL_STATE::NONE == m_State)
		{
			// 레벨 스테이트 변경
			m_State = _NextState;

			begin();
		}
		// Pause -> Play
		else if (LEVEL_STATE::PAUSE == m_State)
		{
			m_State = _NextState;
		}
	}

	// 플레이 -> 정지 or 일시정지
	else if ((LEVEL_STATE::PLAY == m_State || LEVEL_STATE::NONE == m_State) &&
			 (LEVEL_STATE::STOP == _NextState || LEVEL_STATE::PAUSE == _NextState || LEVEL_STATE::NONE == _NextState))
	{
		// 레벨 스테이트 변경
		m_State = _NextState;

		CTimeMgr::GetInst()->LockDeltaTime(true);

		// 에디터 카메라 모드
		CRenderMgr::GetInst()->ActiveEditorMode(true);
	}
}