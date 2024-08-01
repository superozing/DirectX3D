#pragma once
#include "CRenderComponent.h"
class CMeshRender : public CRenderComponent
{
public:
	virtual void UpdateData() override;
	virtual void finaltick() override;
	virtual void render() override;
	virtual void render(UINT _Subset) override;
	void		 render_AfterImage(int instanceCount);

	CLONE(CMeshRender);

public:
	CMeshRender();
	~CMeshRender();
};
