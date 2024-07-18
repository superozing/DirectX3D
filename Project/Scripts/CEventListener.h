#pragma once
#include <Engine\CScript.h>

struct tListenerData
{
	CGameObject* Target;

	CScript*   Reciever;
	Delegate_S Callback;
};

class CEventListener : public CScript
{
private:
	vector<tListenerData> m_vecEvent;
	bool				  m_bDrawing;

public:
	void PushCallBack(CGameObject* _target, CScript* _receiver, Delegate_S _callback);

	static bool PushCallBack(const wstring& _detectorName, CGameObject* _target, CScript* _receiver,
							 Delegate_S _callback);

public:
	virtual void tick() override;
	virtual void begin() override;

	virtual void BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) override;
	virtual void Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) {}
	virtual void EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) {}

public:
	CLONE(CEventListener);
	CEventListener();
	~CEventListener();

private:
	bool ComponentCheck();

	void Example();
	void eventTestFunc();
};
