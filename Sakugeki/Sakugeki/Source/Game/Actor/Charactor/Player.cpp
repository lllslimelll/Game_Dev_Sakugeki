#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Math/UtilityMath.h"
#include "../../Collider/ColliderLine.h"
#include "../../Collider/ColliderCapsule.h"
#include "../../../Engine/Component/CharacterMovementComponent.h"
#include "../../../Engine/Component/MeshRendererComponent.h"
#include "../../../Engine/Component/AnimatorComponent.h"
#include "../../Component/PlayerControllerComponent.h"
#include "Player.h"

Player::Player(Camera& camera)
	:
	GameObject(),
	movementComp_(nullptr)
{
	AddComponent<PlayerControllerComponent>(camera);
	movementComp_ = AddComponent<CharacterMovementComponent>(transform_, hitColliders_);
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
	AddComponent<MeshRendererComponent>(
		ResourceManager::SRC::SHADER_VS_PLAYER,
		ResourceManager::SRC::SHADER_PS_PLAYER);
}

void Player::InitCollider(void)
{
	// 主に地面との衝突で使用する線分コライダ
	ColliderLine* colLine = new ColliderLine(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_LINE_START_LOCAL_POS, COL_LINE_END_LOCAL_POS);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::GROUND_LINE), colLine);

	// 主に壁や木などの衝突で使用するカプセルコライダ
	ColliderCapsule* colCapsule = new ColliderCapsule(
		ColliderBase::TAG::PLAYER, &transform_,
		COL_CAPSULE_TOP_LOCAL_POS, COL_CAPSULE_DOWN_LOCAL_POS, COL_CAPSULE_RADIUS);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::CAPSULE), colCapsule);

	// 移動の押し戻しにカプセルを使う
	movementComp_->SetCapsule(colCapsule);
}

void Player::InitAnimation(void)
{
	using AnimType = PlayerControllerComponent::ANIM_TYPE;

	// アニメーターコンポーネントを追加（このあとは使わないので、ローカル変数で持つ）
	auto* animator = AddComponent<AnimatorComponent>();

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
