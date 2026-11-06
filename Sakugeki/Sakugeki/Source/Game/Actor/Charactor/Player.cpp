#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Math/UtilityMath.h"
#include "../../../Engine/Component/Collider/LineCollider.h"
#include "../../../Engine/Component/Collider/CapsuleCollider.h"
#include "../../../Engine/Component/Collider/CollisionManager.h"
#include "../../../Engine/Component/MeshRenderer.h"
#include "../../../Engine/Component/Animator.h"
#include "../../../Game/Component/PlayerController.h"
#include "../../../Engine/Component/CharacterMover.h"
#include "../../../Engine/Component/CharacterController.h"
#include "../../../Game/Actor/Charactor/Player.h"

Player::Player(const Camera& camera, CollisionManager& collisionMng)
	:
	GameObject()
{
	AddComponent<PlayerController>(camera);
	auto* mover = AddComponent<CharacterMover>();
	AddComponent<CharacterController>(collisionMng);

	// 線分コライダ
	AddComponent<LineCollider>(collisionMng, Collider::TAG::PLAYER,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);

	// カプセルコライダ
	AddComponent<CapsuleCollider>(collisionMng, Collider::TAG::PLAYER,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS, COL_CAPSULE_RADIUS);

	mover->SetMaxWalkSpeed(MAX_WALK_SPEED);
	mover->SetJumpSpeed(JUMP_SPEED);
}

Player::~Player(void)
{
}

void Player::Load()
{
	// モデル
	transform_.SetModel(resMng_.Load(
		ResourceManager::SRC::PLAYER).handleId_);

	// シェーダー
	resMng_.Load(ResourceManager::SRC::SHADER_VS_PLAYER);
	resMng_.Load(ResourceManager::SRC::SHADER_PS_PLAYER);

	// アニメーション
	resMng_.Load(ResourceManager::SRC::ANIM_IDLE);
	resMng_.Load(ResourceManager::SRC::ANIM_RUN);
	resMng_.Load(ResourceManager::SRC::ANIM_JUMP);
}

void Player::InitTransform(void)
{
	// 大きさ
	transform_.scl = { 1.3f,1.3f,1.3f };

	// Y軸を180度
	transform_.quaRotLocal = Quaternion::AngleAxis(UtilityMath::DegToRadF(180.0f), Vector3::DIR_UP);

	transform_.Update();
}

void Player::InitRenderer()
{
	AddComponent<MeshRenderer>(
		ResourceManager::SRC::SHADER_VS_PLAYER,
		ResourceManager::SRC::SHADER_PS_PLAYER);
}

void Player::InitAnimation(void)
{
	using AnimType = PlayerController::ANIM_TYPE;

	// アニメーターコンポーネントを追加（このあとは使わないので、ローカル変数で持つ）
	auto* animator = AddComponent<Animator>();

	// 再生するアニメーションを登録
	animator->Add(static_cast<int>(AnimType::IDLE), 20.0f, resMng_.Load(ResourceManager::SRC::ANIM_IDLE).handleId_);
	animator->Add(static_cast<int>(AnimType::RUN), 40.0f, resMng_.Load(ResourceManager::SRC::ANIM_RUN).handleId_);
	animator->Add(static_cast<int>(AnimType::JUMP), 20.0f, resMng_.Load(ResourceManager::SRC::ANIM_JUMP).handleId_);

	// 最初は待機
	animator->Play(static_cast<int>(AnimType::IDLE), true);
}

void Player::InitPost(void)
{
	transform_.Update();
}
