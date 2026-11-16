#pragma once
#include "Component.h"

// メッシュを担当するコンポーネントクラス
class Mesh : public Component
{
public:

	// コンストラクタ
	explicit Mesh(GameObject& gameObject);

	// デストラクタ
	~Mesh(void) override;

	// 初期化（最初の位置をモデルに反映する。MeshCollider の衝突情報の構築より前に必要）
	void Init(void) override;

	// 更新（その時点の位置をモデルに反映する）
	void Update(void) override;

	// モデルのハンドルを設定
	void SetHandle(int handle);

	// モデルのハンドル
	int GetHandle(void) const;

	// モデルを持っているか
	bool HasHandle(void) const;

private:

	// モデルのハンドル
	int handle_;

	// 持ち主の Transform をモデルに反映する
	void ApplyTransform(void);
};