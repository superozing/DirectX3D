#pragma once
#include <Engine\CScript.h>

#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CMesh.h>

class CHaloScript : public CScript
{
private:
	CGameObject*		   m_Parent;
	const vector<tMTBone>* m_ParentBones;
	int					   m_HeadBoneIdx;

	int m_CurClipIdx;
	int m_PrevClipIdx;
	int m_CurFrame;

	Vec3 m_HaloPos;
	Vec3 m_vHeadBonePos;

public:
	bool IsAnimChange();
	void CheckHeadBonePos();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CHaloScript);
	CHaloScript();
	~CHaloScript();
};
