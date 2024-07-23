#pragma once

enum class TASK_TYPE
{
	// Param1 : Layer Index, Param2 : Object Adress, Param3 : IsPrefab
	CREATE_OBJECT,

	// Param1 : Object Adress
	DELETE_OBJECT,

	// Param1 : Asset Adress
	ADD_ASSET,

	// Param1 : AssetType, Param2 : Asset Adress
	DELETE_ASSET,

	// Param1 : Level, Param2 : LEVEL_STATE
	CHANGE_LEVELSTATE,

	// Param1 : Level Adress, Param2 : Next Level Start State
	CHANGE_LEVEL,

	// Param1 : Level Adress
	SAVE_CHECKPOINT,

	// Param1 : Parent Object, Param2 : Child Object
	ADD_CHILD,

	// Param1 : Parent Object, Param2 : Child Object
	DISCONNECT_PARENT,

	// Param1 : ResolutionX , Param2 : ResolutionY
	CHANGE_RESOLUTION,
};

struct tTask
{
	TASK_TYPE Type;
	UINT_PTR  Param_1;
	UINT_PTR  Param_2;
	UINT_PTR  Param_3;
};

class CTaskMgr : public CManager<CTaskMgr>
{
	SINGLE(CTaskMgr);

private:
	vector<tTask> m_vecTask;

	bool m_bCreateObject;
	bool m_bDeleteObject;
	bool m_bAssetChange;
	bool m_bMoveObject;

	int m_DeleteFrameCount;

public:
	virtual void tick() override;
	virtual void enter() override {}

	void AddTask(const tTask& _Task) { m_vecTask.push_back(_Task); }
	bool GetObjectEvent() { return m_bCreateObject || m_bDeleteObject || m_bMoveObject; }
	bool GetAssetEvent() { return m_bAssetChange; }

	void SetMemoryPoolEvent(bool bEvent) { m_bMoveObject = bEvent; }

private:
	void Clear();
};
