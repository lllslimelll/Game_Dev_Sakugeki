#pragma once
#include <functional>
#include "Component.h"

/// 体力を担当するコンポーネントクラス
class HealthComponent : public Component
{
public:

	// 死亡時に呼ばれる関数
	using DeathCallback = std::function<void(void)>;

	// コンストラクタ
	// owner : 持ち主
	// maxHp : 最大体力
	HealthComponent(Actor& owner, int maxHp);

	// デストラクタ
	~HealthComponent(void) override;

	// 初期化
	void Init(void) override;

	// ダメージを受ける
	void TakeDamage(int damage);

	// 回復する
	void Heal(int amount);

	// 死亡時に呼ばれる関数を設定
	void SetOnDeath(DeathCallback onDeath);

	// 体力
	int GetHp(void) const;

	// 最大体力
	int GetMaxHp(void) const;

	// 死亡しているか
	bool IsDead(void) const;

private:

	// 最大体力
	int maxHp_;

	// 体力
	int hp_;

	// 死亡時に呼ばれる関数
	DeathCallback onDeath_;
};