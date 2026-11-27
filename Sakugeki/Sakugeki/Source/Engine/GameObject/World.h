#pragma once
#include <memory>
#include <vector>

class GameObject;
class CollisionManager;

// ゲームの空間
// GameObject を持ち、Load / Init / Update / Draw / Release を、入れた順にまとめて呼ぶ
// 当たり判定（CollisionManager）も持つ
class World
{
public:

	World(void);
	~World(void);

	// 中身ごとコピーされると困るのでコピー禁止
	World(const World&) = delete;
	World& operator=(const World&) = delete;

	// オブジェクトを追加
	GameObject& AddObject(std::unique_ptr<GameObject> object);

	// 全オブジェクトの素材の読み込みを発行
	void Load(void);

	// 全オブジェクトの初期化
	void Init(void);

	// 全オブジェクトの更新
	void Update(void);

	// 全オブジェクトの描画
	void Draw(void);

	// 全オブジェクトの後始末
	void Release(void);

	// この World の当たり判定を取得
	CollisionManager& GetCollisionManager(void);

private:

	// 当たり判定
	std::unique_ptr<CollisionManager> collisionMng_;

	// オブジェクト
	std::vector<std::unique_ptr<GameObject>> objects_;
};