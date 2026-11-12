#pragma once
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/GameObject/GameObject.h"

class Camera;
class CollisionManager;

class Player : public GameObject
{
public:

	// camera : 移動方向の基準にするカメラ
	// collisionMng : コライダーの登録先
	explicit Player(const Camera& camera, CollisionManager& collisionMng);

	~Player(void) override;

	// リソースロード
	void Load() override;

private:

	// 初期化
	// 大きさ、回転、座標
	void InitTransform() override;
	// 描画設定
	void InitRenderer() override;
	// アニメーション
	void InitAnimation() override;
	// 個別処理
	void InitPost() override;

	// 移動速度（単位 / 秒）
	static constexpr float MAX_WALK_SPEED = 600.0f;

	// ジャンプの初速（単位 / 秒）
	static constexpr float JUMP_SPEED = 350.0f;

	// 最大体力
	static constexpr int MAX_HP = 100;

	// 衝突判定用カプセル上部球体
	static inline const Vector3 COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };
	// 衝突判定用カプセル下部球体
	static inline const Vector3 COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };
	// 衝突判定用カプセル
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;
};
