#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Math/UtilityMath.h"
#include "../../../Engine/Collider/LineCollider.h"
#include "../../../Engine/Collider/CapsuleCollider.h"
#include "../../../Engine/Collider/CollisionManager.h"
#include "../../../Engine/Component/CharacterMovement.h"
#include "../../../Engine/Component/MeshRenderer.h"
#include "../../../Engine/Component/Animator.h"
#include "../../Component/PlayerControllerComponent.h"
#include "Player.h"

Player::Player(const Camera& camera, const CollisionManager& collisionMng)
	:
	GameObject(),
	movementComp_(nullptr)
{
	AddComponent<PlayerControllerComponent>(camera);
	movementComp_ = AddComponent<CharacterMovement>(transform_, collisionMng);
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

void Player::InitCollider(void)
{
	// 主に地面との衝突で使用する線分コライダ
	LineCollider* colLine = new LineCollider(
		Collider::TAG::PLAYER, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::GROUND_LINE), colLine);

	// 主に壁や木などの衝突で使用するカプセルコライダ
	CapsuleCollider* colCapsule = new CapsuleCollider(
		Collider::TAG::PLAYER, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS, COL_CAPSULE_RADIUS);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

	// 移動の押し戻しにカプセルを使う
	movementComp_->SetCapsule(colCapsule);
}

void Player::InitAnimation(void)
{
	using AnimType = PlayerControllerComponent::ANIM_TYPE;

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
	// 移動のパラメータ
	movementComp_->SetMaxWalkSpeed(MAX_WALK_SPEED);
	movementComp_->SetJumpSpeed(JUMP_SPEED);

	transform_.Update();
}
