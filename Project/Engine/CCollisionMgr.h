#pragma once

class CCollider2D;

union CollisionID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	UINT_PTR id;
};


class CCollisionMgr
	: public CManager<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_matrix[(UINT)LAYER::LAYER_MAX];
	map<UINT_PTR, bool>		m_mapPrevInfo;		// 이전 프레임에 두 충돌체의 충돌여부

public:
	void LayerCheck(UINT _left, UINT _right);
	void LayerCheck(const wstring& _LeftLayer, const wstring& _RightLayer);

	void Clear()
	{
		for (int i = 0; i < (UINT)LAYER::LAYER_MAX; ++i)
		{
			m_matrix[i] = 0;
		}
	}

public:
	virtual void tick() override;
	virtual void enter() override {}
	virtual void exit() override;

private:
	void CollisionBtwLayer(UINT _left, UINT _right);
	bool CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight);		// 사각형 충돌체끼리 충돌 검사
	bool CollisionBtwCirCleCollider(CCollider2D* _pLeft, CCollider2D* _pRight);	// 원형 충돌체끼리 충돌 검사
};

