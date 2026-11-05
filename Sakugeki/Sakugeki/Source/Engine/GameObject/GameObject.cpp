#include "../../Engine/Resource/ResourceManager.h"
#include "../../Engine/Scene/SceneManager.h" 
#include "GameObject.h"

GameObject::GameObject()
	: 
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	transform_()
{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
	// Transform初期化
	InitTransform();

	// 描画設定の初期化
	InitRenderer();

	// アニメーションの初期化
	InitAnimation();

	// 初期化後の個別処理
	InitPost();

	// コンポーネントの初期化
	for (auto& component : components_)
	{
		component->Init();
	}
}

void GameObject::Update()
{
	// コンポーネントの更新
	for (auto& component : components_)
	{
		if (!component->IsEnabled()) { continue; }
		component->Update();
	}

	// モデル制御更新
	transform_.Update();
}

void GameObject::Draw()
{
	// コンポーネントの描画
	for (auto& component : components_)
	{
		if (!component->IsEnabled()) { continue; }
		component->Draw();
	}
}

void GameObject::Release()
{
	// コンポーネントの解放
	for (auto& component : components_)
	{
		component->Release();
	}

	transform_.Release();
}

const Transform& GameObject::GetTransform() const
{
	return transform_;
}

Transform& GameObject::GetTransform()
{
	return transform_;
}
