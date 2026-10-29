#pragma once
#include <vector>
#include <map>
#include <memory>
#include "../../Engine/Common/Transform.h"

class ResourceManager;
class SceneManager;
class ColliderBase;
class Material;
class ModelRenderer;

class ActorBase
{

public:

	// コンストラクタ
	ActorBase();

	// デストラクタ
	virtual ~ActorBase();

	// リソースロード
	virtual void Load() = 0;

	// 初期化
	void Init(void);

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw();

	// 解放
	virtual void Release();

	// 大きさ、回転、座標等の取得
	const Transform& GetTransform() const;

	// 自身の衝突情報取得
	const std::map<int, ColliderBase*>& GetOwnColliders() const
	{
		return ownColliders_;
	}

	// 特定の自身の衝突情報
	const ColliderBase* GetOwnCollider(int key) const;

	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

	// 衝突対象となるコライダをクリア
	void ClearHitCollider();

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform transform_;

	// モデル描画用
	std::unique_ptr<Material> material_;
	std::unique_ptr<ModelRenderer> renderer_;

	// 自身の衝突情報
	std::map<int, ColliderBase*> ownColliders_;

	// 衝突相手の情報
	std::vector<const ColliderBase*> hitColliders_;

	// 大きさ、回転、座標の初期化
	virtual void InitTransform() = 0;

	// 描画設定の初期化
	virtual void InitRenderer() = 0;

	// 衝突判定の初期化
	virtual void InitCollider() = 0;

	// アニメーションの初期化
	virtual void InitAnimation() = 0;

	// 初期化後の個別処理
	virtual void InitPost() = 0;
};
