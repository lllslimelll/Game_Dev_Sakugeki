#include <memory>
#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Common/AnimationController.h"
#include "../../../Engine/Renderer/Material.h"
#include "../../../Engine/Renderer/ModelRenderer.h"
#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Math/UtilityMath.h"
#include "../../Collider/ColliderLine.h"
#include "../../Collider/ColliderCapsule.h"
#include "../../../Engine/Component/CharacterMovementComponent.h"
#include "Player.h"

Player::Player(void)
	:
	Character()
{
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

void Player::UpdateAnimation(void)
{
	// 移動の状態から選ぶ
	ANIM_TYPE type = ANIM_TYPE::IDLE;
	if (!movementComp_->IsGrounded())
	{
		type = ANIM_TYPE::JUMP;
	}
	else if (movementComp_->IsMoving())
	{
		type = ANIM_TYPE::RUN;
	}

	// ジャンプだけはループしない
	bool isLoop = (type != ANIM_TYPE::JUMP);
	animController_->Play(static_cast<int>(type), isLoop);
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
	material_ = std::make_unique<Material>(ResourceManager::SRC::SHADER_VS_PLAYER, 0,
		ResourceManager::SRC::SHADER_PS_PLAYER, 0);

	renderer_ = std::make_unique<ModelRenderer>(*material_, transform_.modelId);
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
	// アニメーション
	animController_ = std::make_unique<AnimationController>(transform_.modelId);

	// アニメーション
	animController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 20.0f, resMng_.Load(ResourceManager::SRC::ANIM_IDLE).handleId_);
	animController_->Add(static_cast<int>(ANIM_TYPE::RUN), 40.0f, resMng_.Load(ResourceManager::SRC::ANIM_RUN).handleId_);
	animController_->Add(static_cast<int>(ANIM_TYPE::JUMP), 20.0f, resMng_.Load(ResourceManager::SRC::ANIM_JUMP).handleId_);
}

void Player::InitPost(void)
{
	// 移動のパラメータ
	movementComp_->SetMaxWalkSpeed(MAX_WALK_SPEED);
	movementComp_->SetJumpSpeed(JUMP_SPEED);

	transform_.Update();

	animController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}
