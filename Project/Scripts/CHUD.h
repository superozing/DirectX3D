#pragma once
#include <Engine/CScript.h>
class CHUD : public CScript
{
public:
	template <typename T> T* GetHUD();

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CLONE(CHUD);
	CHUD();
	~CHUD();
};

template <typename T> inline T* CHUD::GetHUD()
{
	CScript* pScript = nullptr;

	const auto& childs = GetOwner()->GetChild();
	for (const auto& child : childs)
	{
		pScript = child->GetScript<T>();
		if (pScript)
		{
			return (T*)pScript;
		}
	}

	return (T*)pScript;
}
