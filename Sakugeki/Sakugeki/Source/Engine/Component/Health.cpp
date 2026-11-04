#include <algorithm>
#include "Health.h"

Health::Health(GameObject& gameObject, int maxHp)
	:
	Component(gameObject),
	maxHp_(maxHp),
	hp_(maxHp),
	onDeath_()
{
}

Health::~Health(void)
{
}

void Health::Init(void)
{
	hp_ = maxHp_;
}

void Health::TakeDamage(int damage)
{
	if (IsDead() || damage <= 0) { return; }

	hp_ = (std::max)(hp_ - damage, 0);

	// 体力が 0 になったら死亡を通知
	if (IsDead() && onDeath_)
	{
		onDeath_();
	}
}

void Health::Heal(int amount)
{
	if (IsDead() || amount <= 0) { return; }

	hp_ = (std::min)(hp_ + amount, maxHp_);
}

void Health::SetOnDeath(DeathCallback onDeath)
{
	onDeath_ = std::move(onDeath);
}

int Health::GetHp(void) const
{
	return hp_;
}

int Health::GetMaxHp(void) const
{
	return maxHp_;
}

bool Health::IsDead(void) const
{
	return hp_ <= 0;
}