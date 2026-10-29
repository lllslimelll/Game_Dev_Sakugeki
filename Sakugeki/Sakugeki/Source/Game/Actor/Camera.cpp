#include <algorithm>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Engine/Input/Input.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Common/Transform.h"
#include "../../Engine/Math/UtilityMath.h"
#include "../Collider/ColliderBase.h"
#include "../Collider/ColliderModel.h"
#include "../Collider/ColliderSphere.h"
#include "Camera.h"

Camera::Camera(void)
	:
	followTransform_(nullptr),
	mode_(MODE::NONE),
	angles_(Vector3::ZERO),
	rotY_(Quaternion::IDENTITY),
	targetPos_(Vector3::ZERO)
{
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
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Camera::InitCollider(void)
{
	// 主にステージとの衝突で使用する球体コライダ
	ColliderSphere* colliderSphere = new ColliderSphere(
		ColliderBase::TAG::CAMERA,
		&transform_,
		Vector3::ZERO,
		COL_CAPSULE_SPHERE);

	// 自分自身の衝突情報に登録
	ownColliders_.emplace(
		static_cast<int>(COLLIDER_TYPE::SPHERE), colliderSphere);
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

	for (const auto& hitCol : hitColliders_)
	{
		// モデル以外は処理を飛ばす
		if (hitCol->GetShape() != ColliderBase::SHAPE::MODEL) continue;

		// 派生クラスへキャスト
		const ColliderModel* colliderModel =
			dynamic_cast<const ColliderModel*>(hitCol);

		if (colliderModel == nullptr) continue;

		// 線分とモデルの最近接(startに近い)衝突ポリゴンを取得
		auto hitPoly = colliderModel->GetNearestHitPolyLine(
			start, transform_.pos,
			false, //ブラックリストを使うか否か
			true   // ホワイトリストを使うか否か
		);

		// 線分が衝突していなければ次のコライダへ
		if (!hitPoly.HitFlag) continue;

		// カメラ位置から注視点への方向
		Vector3 dirToTarget = (targetPos_ - transform_.pos).Normalized();

		// 衝突点の少し手前にカメラを置く
		transform_.pos =
			Vector3::GetVector3(hitPoly.HitPosition) + (dirToTarget * COLLISION_BACK_DIS);

#pragma region 球体の衝突で線分衝突の補助として押し戻す

		// カメラ位置の球体コライダ
		int typeSphere = static_cast<int>(COLLIDER_TYPE::SPHERE);
		// 球体コライダがなければ処理を抜ける
		if (ownColliders_.count(typeSphere) == 0) continue;

		// 指定された回数と距離で三角形の法線方向に押し戻す
		transform_.pos =
			ownColliders_.at(typeSphere)->GetPosPushBackAlongNormal(
				hitPoly, CNT_TRY_COLLISION_CAMERA, COLLISION_BACK_DIS);

#pragma endregion
	}
}

Vector2 Camera::GetRotMouse()
{
	int screenW, screenH = 0;
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
