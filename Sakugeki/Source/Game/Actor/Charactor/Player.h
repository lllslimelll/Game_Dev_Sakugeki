#pragma once
#include "CharactorBase.h"

class Player : public CharactorBase
{
public:

	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		JUMP,
	};

	Player(void);

	~Player(void) override;

	// リソースロード
	virtual void Load() override;

private:

	// 移動速度
	static constexpr float SPEED_MOVE = 10.0f;

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };
	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };
	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };
	// 衝突判定用カプセル
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;

	// ジャンプ力
	static constexpr float POW_JUMP_INIT = 350.0f;

	// 初期化
	// 大きさ、回転、座標
	void InitTransform() override;
	// 描画設定
	void InitRenderer() override;
	// 衝突判定
	void InitCollider() override;
	// アニメーション
	void InitAnimation() override;
	// 個別処理
	void InitPost() override;

	// 移動処理
	void ProcessMove(void);
	void ProcessJump(void);

	// 更新系
	virtual void UpdateProcess(void) override;

	// 衝突判定前準備
	void CollisionReserve(void) override;
};

