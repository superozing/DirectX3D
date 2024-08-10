#pragma once
#include <Engine/CScript.h>

// OBB (Oriented Bounding Box) 구조체
struct OBB
{
	Vector3				   center;
	Vector3				   halfSizes;
	std::array<Vector3, 3> axes; // Local axes

	Vector3 getVertex(int index) const
	{
		Vector3 vertex = center;
		if (index & 1)
			vertex = vertex + axes[0] * halfSizes.x;
		else
			vertex = vertex - axes[0] * halfSizes.x;
		if (index & 2)
			vertex = vertex + axes[1] * halfSizes.y;
		else
			vertex = vertex - axes[1] * halfSizes.y;
		if (index & 4)
			vertex = vertex + axes[2] * halfSizes.z;
		else
			vertex = vertex - axes[2] * halfSizes.z;
		return vertex;
	}
};

class CCubePushScript : public CScript
{
private:
	// 축을 기준으로 투영된 크기를 구하는 함수
	float getProjectionRadius(const OBB& box, const Vector3& axis)
	{
		return box.halfSizes.x * std::abs(axis.Dot(box.axes[0])) + box.halfSizes.y * std::abs(axis.Dot(box.axes[1])) +
			   box.halfSizes.z * std::abs(axis.Dot(box.axes[2]));
	}

	// 파고든 깊이를 계산하는 함수
	Vector3 getPenetrationDepth(const OBB& player, const OBB& wall)
	{
		// 충돌 감지와 유사한 방식으로 할 수 있지만, 여기서는 간단하게 처리합니다.
		Vector3 penetration = {0, 0, 0};

		// 각 축에 대해 깊이를 계산합니다.
		for (const auto& axis : player.axes)
		{
			float playerProj = getProjectionRadius(player, axis);
			float wallProj	 = getProjectionRadius(wall, axis);
			float distance	 = std::abs((wall.center - player.center).Dot(axis));

			// 파고든 깊이를 계산합니다.
			float overlap = (playerProj + wallProj) - distance;
			if (overlap > 0)
			{
				penetration = penetration + axis * overlap;
			}
		}

		return penetration;
	}

	// 플레이어를 밀어주는 함수
	void resolveCollision(OBB& player, const OBB& wall, const Vector3& dir, float threshold)
	{
		Vector3 penetration = getPenetrationDepth(player, wall);

		// 파고든 깊이가 threshold 이상인 경우
		if (std::abs(penetration.x) > threshold || std::abs(penetration.y) > threshold ||
			std::abs(penetration.z) > threshold)
		{
			// dir 방향의 반대로 밀어줌
			Vector3 penetration_dir = {-dir.x, -dir.y, -dir.z};
			player.center =
				player.center +
				(penetration_dir * max(max(std::abs(penetration.x), std::abs(penetration.y)), std::abs(penetration.z)));
		}
	}

public:
	virtual void begin() override;
	virtual void tick() override;
	virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) override;
	virtual void SaveToFile(ofstream& fout);
	virtual void LoadFromFile(ifstream& fin);

public:
	CLONE(CCubePushScript);
	CCubePushScript();
	~CCubePushScript();
};
