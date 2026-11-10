#include <cassert>
#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/GameObject/GameObject.h"
#include "../../../Engine/Component/Collider/CollisionManager.h"
#include "../../../Engine/Component/Collider/Collider.h"


Collider::Collider(GameObject& gameObject, CollisionManager& collisionMng,
				   SHAPE shape, int layer)
	:
	Component(gameObject),
	collisionMng_(collisionMng),
	shape_(shape),
	layer_(layer)
{
	assert(0 <= layer && layer < CollisionManager::MAX_LAYERS && "layer の範囲外です");
}

Collider::~Collider(void)
{
	collisionMng_.Unregister(this);
}

void Collider::Init(void)
{
	collisionMng_.Register(this);
}

void Collider::Draw(void)
{
#ifdef _DEBUG
	DrawDebug(COLOR_DEBUG);
#endif
}

void Collider::Release()
{
	collisionMng_.Unregister(this);
}

Collider::SHAPE Collider::GetShape(void) const
{
	return shape_;
}

int Collider::GetLayer(void) const
{
	return layer_;
}

const Transform& Collider::GetTransform(void) const
{
	return gameObject_.GetTransform();
}

Vector3 Collider::GetRotPos(const Vector3& localPos) const
{
	// 追従相手の回転に合わせて指定ローカル座標を回転し、
	// 基準座標に加えることでワールド座標へ変換
	const Transform& transform = GetTransform();

	return transform.pos + transform.quaRot.Rotate(localPos);
}