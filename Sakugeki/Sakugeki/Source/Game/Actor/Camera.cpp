#include <algorithm>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Engine/Input/Input.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Common/Transform.h"
#include "../../Engine/Math/UtilityMath.h"
#include "../../Engine/Component/Collider/Collider.h"
#include "../../Engine/Component/Collider/MeshCollider.h"
#include "../../Engine/Component/Collider/SphereCollider.h"
#include "../../Engine/Component/Collider/CollisionManager.h"
#include "../../Engine/Component/Collider/ContactPoint.h"
#include "../../Game/Common/CollisionLayer.h"
#include "Camera.h"

Camera::Camera(CollisionManager& collisionMng)
	:
	collisionMng_(collisionMng),
	followTransform_(nullptr),
	mode_(MODE::NONE),
	angles_(Vector3::ZERO),
	rotY_(Quaternion::IDENTITY),
	targetPos_(Vector3::ZERO),
	sphereCollider_(nullptr)
{
	sphereCollider_ = AddComponent<SphereCollider>(
		collisionMng, CollisionLayer::CAMERA,
		Vector3::ZERO,
		COL_SPHERE_SPHERE);
}

Camera::~Camera(void)
{
}

void Camera::Update(void)
{
	// 更新前情報
	prePos_ = transform_.pos;

	switch (mode_)
	{
	case MODE::FOLLOW: UpdateFollow(); break;
	default: break;   // FIXED_POINT は何もしない
	}
}

void Camera::UpdateFollow()
{
	ProcessRot(true);
	SyncFollow();
	Collision();
}

void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(VIEW_NEAR, VIEW_FAR);

	SetCameraPositionAndTargetAndUpVec(
		transform_.pos.ToVECTOR(), targetPos_.ToVECTOR(), transform_.quaRot.GetUp().ToVECTOR());

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

void Camera::DrawDebug(void)
{
}

void Camera::Release(void)
{
	GameObject::Release();
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Camera::InitPost(void)
{
	ChangeMode(MODE::FIXED_POINT);
}

const Vector3& Camera::GetPos(void) const
{
	return transform_.pos;
}

const Vector3& Camera::GetAngles(void) const
{
	return angles_;
}

const Vector3& Camera::GetTargetPos(void) const
{
	return targetPos_;
}

const Quaternion& Camera::GetQuaRot(void) const
{
	return transform_.quaRot;
}

const Quaternion& Camera::GetQuaRotY(void) const
{
	return rotY_;
}

Vector3 Camera::GetForward(void) const
{
	return (targetPos_ - transform_.pos).Normalized();
}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		break;
	}

}

void Camera::SetDefault(void)
{
	// カメラの初期設定
	transform_.pos = DERFAULT_POS;

	// カメラ角
	angles_ = DERFAULT_ANGLES;
	transform_.quaRot = Quaternion::IDENTITY;

	// 注視点
	targetPos_ = Vector3::ZERO;
}

void Camera::SyncFollow(void)
{

	// 同期先の位置
	Vector3 pos = followTransform_->pos;

	// Y軸
	rotY_ = Quaternion::AngleAxis(angles_.y, Vector3::DIR_UP);

	// Y軸 + X軸
	transform_.quaRot = rotY_ * Quaternion::AngleAxis(angles_.x, Vector3::DIR_RIGHT);

	// 注視点
	targetPos_ = pos + transform_.quaRot.Rotate(FOLLOW_TARGET_LOCAL_POS);

	// カメラ位置
	transform_.pos = pos + transform_.quaRot.Rotate(FOLLOW_CAMERA_LOCAL_POS);
}

void Camera::ProcessRot(bool isLimit)
{
	// 回転量
	Vector2 rot = GetRotMouse() + GetRotPad();

	angles_.y += rot.x;
	angles_.x += rot.y;

	// 角度制限（上下を見すぎないようにする）
	if (isLimit)
	{
		angles_.x = std::clamp(angles_.x, -LIMIT_X_DW_RAD, LIMIT_X_UP_RAD);
	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}

void Camera::SetBeforeDrawFollow(void)
{
	// カメラ操作(回転)
	ProcessRot(true);

	// 追従対象との相対位置を同期
	SyncFollow();

	// 衝突判定
	Collision();
}

void Camera::Collision(void)
{
	// プレイヤーのルートフレーム
	Vector3 start = Vector3::GetVector3(MV1GetFramePosition(followTransform_->modelId, 1));

	// プレイヤーとの間を遮るもの
	for (const Collider* hitCol : collisionMng_.GetColliders())
	{
		// コライダーが無効・メッシュ以外・当たらないレイヤーなら無視する
		if (!hitCol->IsEnabled() ||
			hitCol->GetShape() != Collider::SHAPE::MESH ||
			!collisionMng_.CanCollide(*sphereCollider_, *hitCol))
		{
			continue;
		}

		const auto* mesh = static_cast<const MeshCollider*>(hitCol);

		// 線分とメッシュの最近接(startに近い)衝突ポリゴンを取得
		auto hitPoly = mesh->GetNearestHitPolyLine(
			start, transform_.pos,
			false,	// ブラックリストを使うか否か
			false	// ホワイトリストを使うか否か
		);

		// 線分が衝突していなければ次のコライダへ
		if (!hitPoly.HitFlag) { continue; }

		// カメラ位置から注視点への方向
		Vector3 dirToTarget = (targetPos_ - transform_.pos).Normalized();

		// 衝突点の少し手前にカメラを置く
		transform_.pos =
			Vector3::GetVector3(hitPoly.HitPosition) + (dirToTarget * COLLISION_BACK_DIS);
	}

	// めり込みの解消（球）
	ResolvePenetration();
}

void Camera::ResolvePenetration(void)
{
	for (int i = 0; i < MAX_RESOLVE_ITERATIONS; i++)
	{
		bool isHitAny = false;

		for (const Collider* hitCol : collisionMng_.GetColliders())
		{
			// コライダーが無効・自分のコライダー・当たらないレイヤーなら無視する
			if (!hitCol->IsEnabled() ||
				&hitCol->GetGameObject() == this ||
				!collisionMng_.CanCollide(*sphereCollider_, *hitCol)) 
			{ 
				continue; 
			}

			// 検出
			ContactPoint contact;
			if (!collisionMng_.ComputePenetration(*sphereCollider_, *hitCol, contact)) { continue; }

			// めり込んだ分だけ押し戻す
			transform_.pos += contact.normal * contact.depth;
			isHitAny = true;
		}

		// どれにも当たらなくなったら終了
		if (!isHitAny) { break; }
	}
}

Vector2 Camera::GetRotMouse()
{
	int screenW = 0, screenH = 0;
	GetDrawScreenSize(&screenW, &screenH);
	Vector2 center(screenW / 2.0f, screenH / 2.0f);

	// 画面中心からの移動量
	Vector2 delta = Input::GetInstance().GetMousePos() - center;

	// マウスカーソルを画面の中心に戻す
	SetMousePoint(static_cast<int>(center.x), static_cast<int>(center.y));

	// 回転量
	return delta * MOUSE_SENSITIVITY;
}

Vector2 Camera::GetRotPad()
{
	// スティックの入力方向
	Vector2 stick = Input::GetInstance().GetRightStickDirection();

	// 1フレームの回転量（rad）
	float rotPow = UtilityMath::DegToRadF(PAD_ROT_SPEED_DEG) * scnMng_.GetDeltaTime();

	// 回転量
	return Vector2(stick.x, -stick.y) * rotPow;
}
