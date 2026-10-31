#include <algorithm>
#include "HealthComponent.h"

HealthComponent::HealthComponent(GameObject& gameObject, int maxHp)
	:
	Component(gameObject),
	maxHp_(maxHp),
	hp_(maxHp),
	onDeath_()
{
}

HealthComponent::~HealthComponent(void)
{
}

void HealthComponent::Init(void)
{
	hp_ = maxHp_;
}

void HealthComponent::TakeDamage(int damage)
{
	if (IsDead() || damage <= 0) { return; }

	hp_ = (std::max)(hp_ - damage, 0);

	// ‘Ì—Í‚ª 0 ‚É‚È‚Á‚½‚çŽ€–S‚ð’Ê’m
	if (IsDead() && onDeath_)
	{
		onDeath_();
	}
}

void HealthComponent::Heal(int amount)
{
	if (IsDead() || amount <= 0) { return; }

	hp_ = (std::min)(hp_ + amount, maxHp_);
}

void HealthComponent::SetOnDeath(DeathCallback onDeath)
{
	onDeath_ = std::move(onDeath);
}

int HealthComponent::GetHp(void) const
{
	return hp_;
}

int HealthComponent::GetMaxHp(void) const
{
	return maxHp_;
}

bool HealthComponent::IsDead(void) const
{
	return hp_ <= 0;
}