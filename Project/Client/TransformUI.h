#pragma once
#include "ComponentUI.h"

#include <Engine/CGameObject.h>

class TransformUI : public ComponentUI
{
public:
	virtual void render_update() override;

public:
	TransformUI();
	~TransformUI();

private:
	void CopyPaste(const string& _id, Vec3& _vec);
};
