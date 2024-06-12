#pragma once
#include "CComponent.h"


#include "PxPhysicsAPI.h"
using namespace physx;

class CPhysX :
    public CComponent
{
private:
    PxRigidActor* mActor;
    void updateFromPhysics();
    void updateToPhysics();

public:
    virtual void begin() override;
    virtual void finaltick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void SaveToFile(ofstream& fout) {};
    virtual void LoadFromFile(FILE* _File) override;
    virtual void LoadFromFile(ifstream& fin) {};

    void setTransform(const PxTransform& transform);
    PxTransform getTransform() const;

    PxRigidActor* getActor() const {
        return mActor;
    }

public:
    CLONE(CPhysX);
    CPhysX();
    //CPhysX(PxRigidActor* actor) : mActor(actor) {}
    ~CPhysX();
};
