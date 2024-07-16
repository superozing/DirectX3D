#pragma once
#include "AssetUI.h"

class MeshDataUI : public AssetUI
{
private:
	class CMeshData* m_pTarget;

public:
	virtual void render_update() override;
	virtual void Activate() override;

	void MeshSelect(DWORD_PTR _ptr);
	void MaterialSelect(DWORD_PTR _ptr, UINT _idx);

public:
	MeshDataUI();
	~MeshDataUI();
};
