﻿#include "pch.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "CComponent.h"

#include "CEngine.h"
#include "CDevice.h"

#include "CLogMgr.h"
#include "CPhysX.h"
CTaskMgr::CTaskMgr()
	: m_bCreateObject(false)
	, m_bDeleteObject(false)
	, m_bMoveObject(false)
{
}

CTaskMgr::~CTaskMgr()
{
}

void CTaskMgr::tick()
{
	Clear();

	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		switch (m_vecTask[i].Type)
		{
		case TASK_TYPE::CREATE_OBJECT: {
			int			 LayerIdx = (int)m_vecTask[i].Param_1;
			CGameObject* Object	  = (CGameObject*)m_vecTask[i].Param_2;
			bool		 IsPrefab = (bool)m_vecTask[i].Param_3;

			if (IsPrefab)
				Object->m_iLayerIdx = -1;

			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject(Object, LayerIdx, false);

			m_bCreateObject = true;

			if (LEVEL_STATE::PLAY == pCurLevel->GetState())
			{
				Object->begin();
			}
		}
		break;
		case TASK_TYPE::DELETE_OBJECT: {
			CGameObject* pDeadObj = (CGameObject*)m_vecTask[i].Param_1;

			list<CGameObject*> queue;
			queue.push_back(pDeadObj);

			// 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
			while (!queue.empty())
			{
				CGameObject* pObject = queue.front();
				queue.pop_front();
				if (nullptr != pObject->PhysX())
				{
					pObject->PhysX()->releaseActor();
				}
				pObject->m_bDead = true;

				for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
				{
					queue.push_back(pObject->m_vecChild[i]);
				}
			}

			if (m_DeleteFrameCount == 0)
				++m_DeleteFrameCount;
			else if (m_DeleteFrameCount == 2)
				m_DeleteFrameCount = 1;
		}
		break;

		case TASK_TYPE::ADD_ASSET: {
			// Param1 : Asset Adress
			CAsset* pAsset = (CAsset*)m_vecTask[i].Param_1;
			CAssetMgr::GetInst()->AddAsset(pAsset->GetName(), pAsset);
			m_bAssetChange = true;
		}
		break;

		case TASK_TYPE::DELETE_ASSET: {
			// Param1 : AssetType, Param2 : Asset Adress
			ASSET_TYPE Type	  = (ASSET_TYPE)m_vecTask[i].Param_1;
			CAsset*	   pAsset = (CAsset*)m_vecTask[i].Param_2;
			CAssetMgr::GetInst()->DeleteAsset(Type, pAsset->GetKey());
			m_bAssetChange = true;
		}

		break;

		case TASK_TYPE::CHANGE_LEVELSTATE: {
			CLevel*		pLevel	  = (CLevel*)m_vecTask[i].Param_1;
			LEVEL_STATE NextState = (LEVEL_STATE)m_vecTask[i].Param_2;

			pLevel->ChangeState(NextState);
		}
		break;

		case TASK_TYPE::CHANGE_LEVEL: {

			string strPrevLevelPath = "";

			if (CLevelMgr::GetInst()->GetCurrentLevel() != nullptr)
			{
				strPrevLevelPath = CLevelMgr::GetInst()->GetCurrentLevel()->GetRelativePath();
			}

			CLevel*		pNextLevel = (CLevel*)m_vecTask[i].Param_1;
			LEVEL_STATE State	   = (LEVEL_STATE)m_vecTask[i].Param_2;
			CLevelMgr::GetInst()->ChangeLevel_Task(pNextLevel, State);
			m_bCreateObject = true;

			CLevelMgr::GetInst()->SetstrPrevLevelPath(strPrevLevelPath);
		}
		break;

		case TASK_TYPE::SAVE_CHECKPOINT: {

			CLevel* pPlayLevel = (CLevel*)m_vecTask[i].Param_1;
			pPlayLevel->ChangeState(LEVEL_STATE::PAUSE);

			CLevelMgr::CheckPointFunc(pPlayLevel);

			pPlayLevel->ChangeState(LEVEL_STATE::PLAY);
		}
		break;

		case TASK_TYPE::ADD_CHILD: {
			CGameObject* pParent = (CGameObject*)m_vecTask[i].Param_1;
			CGameObject* pChild	 = (CGameObject*)m_vecTask[i].Param_2;
			bool		 bSpawn	 = (bool)m_vecTask[i].Param_3;

			pParent->AddChild_RealFunc(pChild, bSpawn);
			m_bCreateObject;
		}
		break;

		case TASK_TYPE::DISCONNECT_PARENT: {
		}
		break;

		case TASK_TYPE::CHANGE_RESOLUTION: {

			float ResX = (float)(m_vecTask[i].Param_1);
			float ResY = (float)m_vecTask[i].Param_2;

			Vec2 vRes = {ResX, ResY};

			bool IsWidnowMode = CDevice::GetInst()->GetScreenMode();

			CEngine::GetInst()->ResizeScreenResolution(vRes, IsWidnowMode);
		}
		break;
		}
	}

	m_vecTask.clear();
}

void CTaskMgr::Clear()
{
	m_bCreateObject = false;
	m_bMoveObject	= false;

	// 억지
	if (1 == m_DeleteFrameCount)
	{
		++m_DeleteFrameCount;
		m_bDeleteObject = true;
	}
	else if (2 <= m_DeleteFrameCount)
	{
		m_DeleteFrameCount = 0;
		m_bDeleteObject	   = false;
	}

	m_bAssetChange = false;
}
