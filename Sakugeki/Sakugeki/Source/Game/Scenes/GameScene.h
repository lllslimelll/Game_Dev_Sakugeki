#pragma once
#include <vector>
#include <memory>
#include "SceneBase.h"

class Camera;
class StageManager;
class Player;
class PlayerController;

class GameScene : public SceneBase
{
public:
	
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	// ロード
	void Load() override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

private:
	
	// カメラ
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<StageManager> stageMng_;
	std::unique_ptr<Player> player_;

	// プレイヤーの操作役
	std::unique_ptr<PlayerController> playerController_;

	// リアルシャドウ描画
	void DrawShadow(void);
};
