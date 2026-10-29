#pragma once
#include <memory>
#include <vector>

class StageGround;
class StageTree;
class ColliderBase;

// ステージ管理クラス
class StageManager
{
public:

	StageManager(void);
	~StageManager(void);

	void Load(void);   
	void Init(void);    
	void Update(void);
	void Draw(void);
	void Release(void);

	// コライダ取得
	std::vector<const ColliderBase*> GetColliders(void) const;

	// モデルハンドル取得
	std::vector<int> GetModelIds(void) const;

private:

	std::unique_ptr<StageGround> ground_;
	std::unique_ptr<StageTree>   tree_;
};