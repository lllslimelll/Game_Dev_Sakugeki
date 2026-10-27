#pragma once
#include <DxLib.h>
#include "ActorBase.h"


class SkyDome : public ActorBase
{

public:

	// 状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	SkyDome(const Transform& followTransform);

	~SkyDome() override;

	// リソースロード
	void Load(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

protected:

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

private:

	// 追従対象のTransform
	const Transform& followTransform_;

	// 状態
	STATE state_;

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateStay(void);
	void ChangeStateFollow(void);

	// 更新
	void UpdateNone(void);
	void UpdateStay(void);
	void UpdateFollow(void);
};