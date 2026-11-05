#pragma once
#include <memory>
#include <vector>

class StageGround;
class StageTree;
class CollisionManager;

// ステージ管理クラス
class StageManager
{
public:

	StageManager(CollisionManager& collisionMng);
	~StageManager();

	void Load();   
	void Init();    
	void Update();
	void Draw();
	void Release();

	// モデルハンドル取得
	std::vector<int> GetModelIds() const;

private:

	std::unique_ptr<StageGround> ground_;
	std::unique_ptr<StageTree>   tree_;
};