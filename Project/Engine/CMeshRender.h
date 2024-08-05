#pragma once
#include "CRenderComponent.h"
class CMeshRender : public CRenderComponent
{
private:
	UINT m_StencilMask = 0;

public:
	virtual void UpdateData() override;
	virtual void finaltick() override;
	virtual void render() override;
	virtual void render(UINT _Subset) override;
	void		 render_AfterImage(int instanceCount);

	UINT GetStencilMask() { return m_StencilMask; }
	void SetStencilMask(UINT _StencilMask) { m_StencilMask = _StencilMask; }

	CLONE(CMeshRender);

public:
	CMeshRender();
	~CMeshRender();
};
