#pragma once
#include "UI.h"
class ModelUI : public UI
{
private:
	CGameObject*  m_pModel;
	class TreeUI* m_pBoneTree;

	float m_fDrawSize;
	bool  m_bDraw;
	bool  m_bDrawOnlySelected;

public:
	void SetModel(CGameObject* _obj);

public:
	virtual void tick() override;
	virtual void render_update() override;

public:
	ModelUI();
	~ModelUI();
};
