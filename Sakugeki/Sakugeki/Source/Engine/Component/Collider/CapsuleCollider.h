#pragma once
#include <DxLib.h>
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

	// 指定された回数と距離で三角形の法線方向に押し戻す
	void PushBackAlongNormal(
		const MeshCollider* colliderModel, Transform& transform,
		int maxTryCnt, float pushDistance,
		bool isExclude = false, bool isTarget = false) const;

	// 指定された回数と距離で三角形の法線方向に押し戻した座標を取得
	Vector3 GetPosPushBackAlongNormal(
		const MV1_COLL_RESULT_POLY& hitPoly,
		int maxTryCnt,
		float pushDistance) const override;

	// メッシュと衝突しているか
	bool IsHit(const MeshCollider* colliderModel,
		bool isExclude = false, bool isTarget = false) const;

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

