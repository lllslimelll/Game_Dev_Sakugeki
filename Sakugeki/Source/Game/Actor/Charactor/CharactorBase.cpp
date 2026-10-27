#include "../../../Engine/Core/Application.h"
#include "../../../Engine/Scene/SceneManager.h"
#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Common/AnimationController.h"
#include "../../../Engine/Utility/AsoUtility.h"
#include "../../Collider/ColliderLine.h"
#include "../../Collider/ColliderModel.h"
#include "../../Collider/ColliderCapsule.h"
#include "../../Collider/ColliderCircleBoundary.h" 
#include "../../Collider/ColliderPlane.h"
#include "CharactorBase.h"

CharactorBase::CharactorBase(void)
	:
	ActorBase(),
	moveDir_(AsoUtility::VECTOR_ZERO),
	moveSpeed_(),
	movePow_(AsoUtility::VECTOR_ZERO),
	animController_()
{
}

CharactorBase::~CharactorBase(void)
{
}

void CharactorBase::Update(void)
{
	// 移動前座標を更新
	prevPos_ = transform_.pos;

	// 各キャラクターごとの更新処理
	UpdateProcess();

	// 移動方向に応じた遅延回転
	DelayRotate();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	// モデル制御更新
	transform_.Update();

	// アニメーション再生
	animController_->Update();
}

void CharactorBase::Release()
{
	// アニメーションのリソース解放
	if (animController_ != nullptr)
	{
		animController_->Release();
	}

	ActorBase::Release();
}

void CharactorBase::UpdateProcess(void)
{
}

void CharactorBase::DelayRotate(void)
{
	if (AsoUtility::EqualsVZero(moveDir_)) return;

	// 移動方向から回転に変換する
	Quaternion goalRot = Quaternion::LookRotation(moveDir_);

	// 回転の補間
	transform_.quaRot =
		Quaternion::Slerp(transform_.quaRot, goalRot, 0.2f);
}

void CharactorBase::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力の強さ
	float gravityPow = (GRAVITY * GRAVITY_SCALE) * scnMng_.GetDeltaTime(); 

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	if (jumpPow_.y < FALL_SPEED_MAX)
	{
		jumpPow_.y = FALL_SPEED_MAX;
	}
}

void CharactorBase::Collision(void)
{
	// 移動処理
	transform_.pos = VAdd(transform_.pos, movePow_);

	// 移動制限（円周）
	CollisionCircleBoundary();

	// 衝突 (カプセル)
	CollisionCapsule();

	// ジャンプ量を加算
	transform_.pos = VAdd(transform_.pos, jumpPow_);

	// 衝突（重力）
	CollisionGravity();
}

void CharactorBase::CollisionGravity(void)
{
	// 地面（平面）との当たり判定
	for (const auto& hitCol : hitColliders_)
	{
		if (hitCol->GetTag() != ColliderBase::TAG::GROUND) continue;
		if (jumpPow_.y > 0.0f) continue;   // 上昇中は判定しない

		bool isHit = false;

		const auto* plane = static_cast<const ColliderPlane*>(hitCol);
		float height = plane->GetHeight(); // 地面の高さ
		if (transform_.pos.y <= height)
		{
			transform_.pos.y = height;
			isHit = true;
		}

		if (isHit) isJump_ = false;

		if (!isJump_)
		{
			jumpPow_ = AsoUtility::VECTOR_ZERO;
		}
	}
}

void CharactorBase::CollisionCircleBoundary(void)
{
	// 自分の体の半径（カプセルから取得）
	float bodyRadius = 0.0f;
	const ColliderBase* own = GetOwnCollider(static_cast<int>(COLLIDER_TYPE::CAPSULE));
	if (own != nullptr)
	{
		const ColliderCapsule* capsule = dynamic_cast<const ColliderCapsule*>(own);
		if (capsule != nullptr)
		{
			bodyRadius = capsule->GetRadius();
		}
	}

	// 移動制限
	for (const auto& hitCol : hitColliders_)
	{
		if (hitCol->GetShape() != ColliderBase::SHAPE::CIRCLE_BOUNDARY) continue;

		const ColliderCircleBoundary* boundary =
			static_cast<const ColliderCircleBoundary*>(hitCol);

		boundary->PushBackInside(transform_.pos, bodyRadius);
	}
}

void CharactorBase::CollisionCapsule(void)
{
	// カプセルコライダ
	int capsuleType = static_cast<int>(COLLIDER_TYPE::CAPSULE);

	// カプセルコライダがなければ処理を抜ける
	if (ownColliders_.count(capsuleType) == 0) return;

	// カプセルコライダ情報
	ColliderCapsule* colliderCapsule =
		dynamic_cast<ColliderCapsule*>(ownColliders_.at(capsuleType));
	if (colliderCapsule == nullptr) return;

	// 登録されている衝突物をすべてチェック
	for (const auto& hitCol : hitColliders_)
	{
		// モデル以外は処理を飛ばす
		if (hitCol->GetShape() != ColliderBase::SHAPE::MODEL) continue;

		// 派生クラスへキャスト
		const ColliderModel* colliderModel =
			dynamic_cast<const ColliderModel*>(hitCol);

		if (colliderModel == nullptr) continue;

		// 指定された回数と距離で三角形の法線方向に押し戻す
		colliderCapsule->PushBackAlongNormal(
			colliderModel, transform_,
			CNT_TRY_COLLISION, DIS_COLLISION_BACK,
			true,	// ブラックリスト使用許可
			false	// ホワイトリスト使用許可
		);
	}
}


