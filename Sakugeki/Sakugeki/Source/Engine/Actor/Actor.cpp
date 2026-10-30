#include "../../Engine/Resource/ResourceManager.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Renderer/Material.h"
#include "../../Engine/Renderer/ModelRenderer.h"
#include "../../Game/Collider/ColliderBase.h"  
#include "Actor.h"

Actor::Actor()
	: 
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	transform_()
{
}

Actor::~Actor()
{
}

void Actor::Init()
{
	// Transform初期化
	InitTransform();

	// 描画設定の初期化
	InitRenderer();

	// 衝突判定の初期化
	InitCollider();

	// アニメーションの初期化
	InitAnimation();

	// 初期化後の個別処理
	InitPost();

	// コンポーネントの初期化
	for (auto& conponent : components_)
	{
		conponent->Init();
	}
}

void Actor::Update()
{
	// コンポーネントの更新
	for (auto& component : components_)
	{
		component->Update();
	}
}

void Actor::Draw()
{
	if (renderer_)
	{
		renderer_->Draw();
	}

	// コンポーネントの描画
	for (auto& component : components_)
	{
		component->Draw();
	}

#ifdef _DEBUG

	// 所有しているコライダの描画
	for (const auto& own : ownColliders_)
	{
		own.second->Draw();
	}

#endif // _DEBUG
}

void Actor::Release()
{
	// コンポーネントの解放
	for (auto& component : components_)
	{
		component->Release();
	}

	transform_.Release();

	// 自身のコライダ解放
	for (auto& own : ownColliders_)
	{
		delete own.second;
	}
	ownColliders_.clear();
}

const Transform& Actor::GetTransform() const
{
	return transform_;
}


// 特定の自身の衝突情報
const ColliderBase* Actor::GetOwnCollider(int key) const
{
	if (ownColliders_.count(key) == 0)
	{
		return nullptr;
	}
	return ownColliders_.at(key);
}

// 衝突対象となるコライダを登録
void Actor::AddHitCollider(const ColliderBase* hitCollider)
{
	for (const auto& c : hitColliders_)
	{
		if (c == hitCollider)
		{
			return;
		}
	}

	hitColliders_.emplace_back(hitCollider);
}

// 衝突対象となるコライダをクリア
void Actor::ClearHitCollider()
{
	hitColliders_.clear();
}
