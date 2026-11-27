#include "../Component/Collider/CollisionManager.h"
#include "GameObject.h"
#include "World.h"

World::World(void)
	:
	collisionMng_(std::make_unique<CollisionManager>()),
	objects_()
{
}

World::~World(void)
{
}

GameObject& World::AddObject(std::unique_ptr<GameObject> object)
{
	objects_.push_back(std::move(object));
	return *objects_.back();
}

void World::Load(void)
{
	for (auto& object : objects_)
	{
		object->Load();
	}
}

void World::Init(void)
{
	for (auto& object : objects_)
	{
		object->Init();
	}
}

void World::Update(void)
{
	for (auto& object : objects_)
	{
		object->Update();
	}

	// 全員が動き終わった後に、コライダーの形をデバッグ表示へ渡す
	collisionMng_->AddDebugShapes();
}

void World::Draw(void)
{
	for (auto& object : objects_)
	{
		object->Draw();
	}
}

void World::Release(void)
{
	for (auto& object : objects_)
	{
		object->Release();
	}
}

CollisionManager& World::GetCollisionManager(void)
{
	return *collisionMng_;
}