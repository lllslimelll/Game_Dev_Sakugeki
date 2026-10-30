#pragma once

class Actor;

/// コンポーネントの基底クラス
class Component
{
public:

	// コンストラクタ
	// owner : 持ち主の Actor
	explicit Component(Actor& owner);

	// デストラクタ
	virtual ~Component(void);

	// コピー禁止（持ち主への参照を持つため）
	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;

	// 初期化
	virtual void Init(void);

	// 更新
	virtual void Update(void);

	// 描画
	virtual void Draw(void);

	// 解放
	virtual void Release(void);

	// 持ち主の取得
	Actor& GetOwner(void);
	const Actor& GetOwner(void) const;

protected:

	// 持ち主
	Actor& owner_;
};