#include "pch.h"
#include "RoRCollisionCallback.h"
#include "CGameObject.h"
#include "CPhysX.h"
#include "CLevelMgr.h"
#include "CLevel.h"

void RoRCollisionCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	if (LEVEL_STATE::PLAY != CLevelMgr::GetInst()->GetCurrentLevel()->GetState())
		return;
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
