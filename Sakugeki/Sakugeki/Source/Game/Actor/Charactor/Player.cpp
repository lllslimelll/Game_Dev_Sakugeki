#include <memory>
#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Common/AnimationController.h"
#include "../../../Engine/Scene/SceneManager.h"
#include "../../../Engine/Renderer/Material.h"
#include "../../../Engine/Renderer/ModelRenderer.h"
#include "../Camera.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Math/UtilityMath.h"
#include "../../Collider/ColliderLine.h"
#include "../../Collider/ColliderCapsule.h"
#include "Player.h"

Player::Player(const Camera& camera)
	:
	CharactorBase(),
	camera_(camera)
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

void Player::ProcessMove(void)
{
	auto& ins = Input::GetInstance();

	// 移動量
	movePow_ = Vector3::ZERO;

	// 移動方向
	Vector2 dir = ins.GetMoveInput();

	// 入力がなければ待機
	if (dir.IsZero())
	{
		if (!isJump_)
		{
			animController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
		}
		return;
	}

	// 移動スピード
	moveSpeed_ = SPEED_MOVE;

	// 移動方向をカメラのY軸回転に合わせる
	Quaternion cameraRot = camera_.GetQuaRotY();
	moveDir_ = cameraRot.Rotate(Vector3(dir.x, 0.0f, dir.y));

	// 移動速度を反映
	movePow_ = moveDir_ * moveSpeed_;

	if (!isJump_)
	{
		// 走りアニメ再生
		animController_->Play(static_cast<int>(ANIM_TYPE::RUN), true);
	}

}

void Player::ProcessJump(void)
{
	auto& ins = Input::GetInstance();

	// ジャンプキーが押されたか
	bool isHitKey = ins.IsPressed(Input::Command::JUMP);

	// 初期ジャンプ処理
	if (isHitKey && !isJump_)
	{
		// ジャンプ量の計算
		float jumpSpeed = POW_JUMP_INIT * scnMng_.GetDeltaTime();
		jumpPow_ = Vector3::DIR_UP * jumpSpeed;
		isJump_ = true;

		// アニメーション再生
		animController_->Play(
			static_cast<int>(ANIM_TYPE::JUMP), false);
	}
}

void Player::UpdateProcess(void)
{
	// 移動処理
	ProcessMove();

	// ジャンプ処理
	ProcessJump();
}

void Player::InitTransform(void)
{
	// 大きさ
	transform_.scl = { 1.5f,1.5f,1.5f };

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
}

void Player::InitAnimation(void)
{
	// アニメーション
	animController_ = std::make_unique<AnimationController>(transform_.modelId);

	// アニメーション
	animController_->Add(static_cast<int>(ANIM_TYPE::IDLE), 20.0f, resMng_.Load(ResourceManager::SRC::ANIM_IDLE).handleId_);
	animController_->Add(static_cast<int>(ANIM_TYPE::RUN), 40.0f, resMng_.Load(ResourceManager::SRC::ANIM_RUN).handleId_);
	animController_->Add(static_cast<int>(ANIM_TYPE::JUMP), 20.0f, resMng_.Load(ResourceManager::SRC::ANIM_JUMP).handleId_);

	//animController_->Add(static_cast<int>(ANIM_TYPE::JUMP), 20.0f, resMng_.Load(ResourceManager::SRC::JUMP_RISING).path_);
}

void Player::InitPost(void)
{
	transform_.Update();

	animController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);
}

