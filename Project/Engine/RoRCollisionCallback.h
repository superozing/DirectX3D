﻿#pragma once
// #include <PxSimulationEventCallback.h>

class RoRCollisionCallback : public PxSimulationEventCallback
{
public:
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {}
	virtual void onWake(PxActor** actors, PxU32 count) override {}
	virtual void onSleep(PxActor** actors, PxU32 count) override {}
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer,
						   const PxU32 count) override
	{
	}

	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;

	void handleBeginOverlap(PxActor* actor1, PxActor* actor2);

	static void handleOverlap(PxActor* actor1, PxActor* actor2);

	void handleEndOverlap(PxActor* actor1, PxActor* actor2);
};
