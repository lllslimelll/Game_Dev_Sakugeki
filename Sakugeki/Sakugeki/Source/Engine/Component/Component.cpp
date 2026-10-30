#include "Component.h"

Component::Component(Actor& owner)
	:
	owner_(owner)
{
}

Component::~Component(void)
{
}

void Component::Init(void)
{
}

void Component::Update(void)
{
}

void Component::Draw(void)
{
}

void Component::Release(void)
{
}

Actor& Component::GetOwner(void)
{
	return owner_;
}

const Actor& Component::GetOwner(void) const
{
	return owner_;
}