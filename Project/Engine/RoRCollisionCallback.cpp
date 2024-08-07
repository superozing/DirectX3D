#include "pch.h"
#include "RoRCollisionCallback.h"
#include "CGameObject.h"
#include "CPhysX.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CPhysXMgr.h"

void RoRCollisionCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	RETURN_IF_NOT_PLAYING

	int	 a		= 0;
	auto actor1 = pairs->triggerActor;
	auto actor2 = pairs->otherActor;

	if (pairs->status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		handleBeginOverlap(actor1, actor2);
		FlagActorColInfo ColInfo = {};
		ColInfo.Actor1			 = actor1;
		ColInfo.Actor2			 = actor2;
		ColInfo.State			 = PxPairFlag::eNOTIFY_TOUCH_FOUND;
		CPhysXMgr::GetInst()->m_vecTriggerColInfo.push_back(ColInfo);
	}
	// Note : Flag방식은 PERSIST이벤트가 들어오지않으므로 별도의 자료구조로 관리해야 한다
	// if (pairs->status & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
	//{
	//	handleOverlap(actor1, actor2);
	// }
	if (pairs->status & PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		handleEndOverlap(actor1, actor2);
		auto iter = CPhysXMgr::GetInst()->m_vecTriggerColInfo.begin();
		while (CPhysXMgr::GetInst()->m_vecTriggerColInfo.end() != iter)
		{
			if (actor1 == iter->Actor1 && actor2 == iter->Actor2)
			{
				iter = CPhysXMgr::GetInst()->m_vecTriggerColInfo.erase(iter);
				break;
			}
			++iter;
		}
	}
}

void RoRCollisionCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	RETURN_IF_NOT_PLAYING

	for (PxU32 i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];

		PxActor* actor1 = pairHeader.actors[0];
		PxActor* actor2 = pairHeader.actors[1];

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			handleBeginOverlap(actor1, actor2);
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			handleOverlap(actor1, actor2);
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			handleEndOverlap(actor1, actor2);
		}
	}
}

void RoRCollisionCallback::handleBeginOverlap(PxActor* actor1, PxActor* actor2)
{
	CGameObject* obj1 = static_cast<CGameObject*>(actor1->userData);
	CGameObject* obj2 = static_cast<CGameObject*>(actor2->userData);
	if (obj1 && obj2 && obj1->PhysX() && obj2->PhysX())
	{
		obj1->PhysX()->BeginOverlap(obj2);
		obj2->PhysX()->BeginOverlap(obj1);
	}
}

void RoRCollisionCallback::handleOverlap(PxActor* actor1, PxActor* actor2)
{
	CGameObject* obj1 = static_cast<CGameObject*>(actor1->userData);
	CGameObject* obj2 = static_cast<CGameObject*>(actor2->userData);
	if (obj1 && obj2 && obj1->PhysX() && obj2->PhysX())
	{
		obj1->PhysX()->Overlap(obj2);
		obj2->PhysX()->Overlap(obj1);
	}
}

void RoRCollisionCallback::handleEndOverlap(PxActor* actor1, PxActor* actor2)
{
	CGameObject* obj1 = static_cast<CGameObject*>(actor1->userData);
	CGameObject* obj2 = static_cast<CGameObject*>(actor2->userData);
	if (obj1 && obj2 && obj1->PhysX() && obj2->PhysX())
	{
		obj1->PhysX()->EndOverlap(obj2);
		obj2->PhysX()->EndOverlap(obj1);
	}
}
