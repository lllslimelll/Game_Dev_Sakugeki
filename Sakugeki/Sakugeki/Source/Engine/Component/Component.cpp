#include "Component.h"

Component::Component(GameObject& gameObject)
	:
	gameObject_(gameObject),
	isEnabled_(true)
{
}

Component::~Component()
{
}

void Component::Init()
{
}

void Component::Update()
{
}

void Component::Draw()
{
}

void Component::Release()
{
}

GameObject& Component::GetGameObject()
{
	return gameObject_;
}

const GameObject& Component::GetGameObject() const
{
	return gameObject_;
}

void Component::SetEnabled(bool isEnabled)
{
	isEnabled_ = isEnabled;
}

bool Component::IsEnabled() const
{
	return isEnabled_;
}
