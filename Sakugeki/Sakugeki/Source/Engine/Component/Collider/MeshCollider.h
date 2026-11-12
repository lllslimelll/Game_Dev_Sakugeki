#pragma once
#include <vector>
#include <string>
#include "../../../Engine/Common/Transform.h"
#include "Collider.h"

class MeshCollider : public Collider
{
public:

	// コンストラクタ
	MeshCollider(GameObject& gameObject, CollisionManager& collisionMng, int layer);
	// デストラクタ
	~MeshCollider(void) override;

	void Init(void) override;

	// 指定されえた文字を含むフレームを衝突判定から除外
	void AddExcludeFrameIds(const std::wstring& name);

	// 衝突判定から除外するフレームをクリアする
	void ClearExcludeFrame(void);

	// 除外フレーム判定
	bool IsExcludeFrame(int frameIdx) const;

protected:

	// 衝突判定から除外するフレーム番号
	std::vector<int> excludeFrameIds_;

	// デバッグ用描画
	void DrawDebug(int color) override {};
};
