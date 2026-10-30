#pragma once
#include <DxLib.h>
#include "Actor.h"

class SkyBox : public Actor
{

public:

	SkyBox();

	~SkyBox() override;

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
	void InitCollider() override {};
	// アニメーション
	void InitAnimation() override {};
	// 個別処理
	void InitPost() override;

private:

};