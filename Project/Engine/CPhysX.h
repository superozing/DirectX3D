#pragma once
#include "CComponent.h"

enum class eColType
{
	COL_NONE,
	COL_BEGINE,
	COL_ON,
	COL_END,
};

enum class PhysShape
{
	BOX,
	SPHERE,
	CONE,
	END,
};

enum class PhysBodyType
{
	STATIC,
	TRIGGER,
	DYNAMIC,
	END,
};

struct tCollisionData
{
	CGameObject* Other;
	eColType	 ColType;
};

class CPhysX : public CComponent
{
private:
	PxRigidActor*	m_Actor					 = nullptr;
	PxRigidDynamic* m_DActor				 = nullptr;
	int				m_CollisionCount		 = 0;
	bool			m_bImguiDirtyFlag		 = false;
	bool			m_bThisFrameImguiFocused = false;
	void			updateFromPhysics();
	void			updateToPhysics();

	bool m_bDrawing;

	void BeginOverlap(CGameObject* other);
	void Overlap(CGameObject* other);
	void EndOverlap(CGameObject* other);

public:
	vector<tCollisionData> m_vThisFrameContact;
	PhysBodyType		   m_bPhysBodyType = PhysBodyType::STATIC;
	PhysShape			   m_Shape		   = PhysShape::BOX;
	Vec3				   m_vScale;
	Vec3				   m_vOffsetPos;
	virtual void		   begin() override;
	virtual void		   finaltick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void SaveToFile(ofstream& fout);
	virtual void LoadFromFile(FILE* _File) override;
	virtual void LoadFromFile(ifstream& fin);

	void		setTransform(const PxTransform& transform);
	PxTransform getTransform() const;
	void		setLinearVelocity(const Vec3& _vLVel);
	void		setAngularVelocity(const Vec3& _vAVel);
	// void		applyBulletImpact(PxRigidDynamic* rigidActor, const PxVec3& bulletVelocity, float bulletMass,
	//							  const PxVec3& hitPoint);
	void applyBulletImpact(const PxVec3& bulletVelocity, float bulletMass, const PxVec3& hitPoint);

	PxRigidActor* getActor() const { return m_Actor; }

public:
	CLONE(CPhysX);
	CPhysX();
	// CPhysX(PxRigidActor* actor) : mActor(actor) {}
	~CPhysX();

	friend class PhysXUI;
	friend class RoRCollisionCallback;
	friend class CPhysXMgr;
	friend class CScript;
};
