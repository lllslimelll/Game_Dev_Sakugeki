#pragma once
#include <memory>
#include "../../Engine/Scene/SceneBase.h"

class World;
class Camera;
class Player;
class HPGauge;

class GameScene : public SceneBase
{
public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	// ロード
	void Load(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

private:

	// ゲーム空間
	std::unique_ptr<World> world_;

	// オブジェクト
	Camera* camera_;
	Player* player_;

	// UI
	std::unique_ptr<HPGauge> hpGauge_;
};