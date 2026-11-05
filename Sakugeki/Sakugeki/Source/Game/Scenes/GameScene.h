#pragma once
#include <vector>
#include <memory>
#include "SceneBase.h"
#include "../UI/DebugHud.h"   

class Camera;
class StageManager;
class Player;
class CollisionManager;

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
	
	std::unique_ptr<CollisionManager> collisionMng_;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<StageManager> stageMng_;
	std::unique_ptr<Player> player_;

	// （既存のメンバの後に追加）
	// TODO(仮UI)
	DebugHud debugHud_;
	HudData debugHudData_;

	// リアルシャドウ描画
	void DrawShadow(void);
	bool IsDebugKeyTriggered(int key);
	bool debugKeyPrev_[];
};
