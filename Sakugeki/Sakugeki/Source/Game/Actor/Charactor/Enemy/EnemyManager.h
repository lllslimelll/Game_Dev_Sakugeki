#pragma once
#include <vector>
#include "Enemy.h"
#include "../../../../Engine/Component/Collider/Collider.h"
class Collider;
class Player;

class EnemyManager
{
public:

	// コンストラクタ
	EnemyManager(Player* player);
	// デストラクタ
	~EnemyManager(void);

	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 解放
	void Release(void);

	// CSV読み込み
	void LoadCsvData(void);

	// エネミー生成
	Enemy* Create(const Enemy::EnemyData& data);

	// エネミー取得
	const std::vector<Enemy*>& GetEnemies(void) const { return enemies_; }

private:

	// プレイヤー
	Player* player_;

	// エネミーリスト
	std::vector<Enemy*> enemies_;
};

