#pragma once

class GameObject;

// コンポーネントの基底クラス
class Component
{
public:

	// コンストラクタ
	// owner : 持ち主の Actor
	explicit Component(GameObject& gameObject);

	// デストラクタ
	virtual ~Component(void);

	// コピー禁止（持ち主への参照を持つため）
	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;

	// 初期化
	virtual void Init();

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();

	// 解放
	virtual void Release();

	// 持ち主の GameObject を取得
	GameObject& GetGameObject();
	const GameObject& GetGameObject() const;

	// 有効・無効の設定
	void SetEnabled(bool isEnabled);

	// 有効か
	bool IsEnabled() const;

protected:

	// 持ち主の GameObject
	GameObject& gameObject_;

private:

	// 有効か
	bool isEnabled_;
};