#pragma once
#include "../../../Engine/Component/Collider/Collider.h"

class Transform;
class MeshCollider;

class CapsuleCollider : public Collider
{
public:

	// コンストラクタ
	CapsuleCollider(GameObject& gameObject, CollisionManager& collisionMng, TAG tag,
		const Vector3& localPosTop, const Vector3& localPosDown, float radius);

	// デストラクタ
	~CapsuleCollider(void) override;

	// 親Transformからの相対位置を取得
	const Vector3& GetLocalPosTop(void) const;
	const Vector3& GetLocalPosDown(void) const;

	// 親Transformからの相対位置をセット
	void SetLocalPosTop(const Vector3& pos);
	void SetLocalPosDown(const Vector3& pos);

	// ワールド座標を取得
	Vector3 GetPosTop(void) const;
	Vector3 GetPosDown(void) const;

	// 半径
	float GetRadius(void) const;
	void SetRadius(float radius);

	// 高さ
	float GetHeight(void) const;

	// カプセルの中心座標
	Vector3 GetCenter(void) const;

protected:

	// デバッグ用描画
	void DrawDebug(int color) override;

private:

	// 親Transformからの相対位置(上端)
	Vector3 localPosTop_;
	// 親Transformからの相対位置(下端)
	Vector3 localPosDown_;

	// 半径
	float radius_;
};

