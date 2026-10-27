#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Engine/Utility/AsoUtility.h"
#include "../../Engine/Input/Input.h"
#include "../Common/Transform.h"
#include "../Collider/ColliderBase.h"
#include "../Collider/ColliderModel.h"
#include "../Collider/ColliderSphere.h"
#include "Camera.h"

Camera::Camera(void)
	:
	followTransform_(nullptr),
	mode_(MODE::NONE),
	angles_(AsoUtility::VECTOR_ZERO),
	rotY_(Quaternion::Identity()),
	targetPos_(AsoUtility::VECTOR_ZERO)
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
	case MODE::FREE:   UpdateFree();   break;
	case MODE::FOLLOW: UpdateFollow(); break;
	default: break;   // FIXED_POINT は何もしない
	}
}

void Camera::UpdateFree()
{
	ProcessRot(false);
	ProcessMove();
	rotY_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);
	transform_.quaRot = rotY_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));
	targetPos_ = VAdd(transform_.pos, transform_.quaRot.PosAxis(FOLLOW_TARGET_LOCAL_POS));
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
		transform_.pos, targetPos_, transform_.quaRot.GetUp());

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
		AsoUtility::VECTOR_ZERO,
		COL_CAPSULE_SPHERE);

	// 自分自身の衝突情報に登録
	ownColliders_.emplace(
		static_cast<int>(COLLIDER_TYPE::SPHERE), colliderSphere);
}

void Camera::InitPost(void)
{
	ChangeMode(MODE::FIXED_POINT);
}

const VECTOR& Camera::GetPos(void) const
{
	return transform_.pos;
}

const VECTOR& Camera::GetAngles(void) const
{
	return angles_;
}

const VECTOR& Camera::GetTargetPos(void) const
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

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, transform_.pos));
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
	case Camera::MODE::FREE:
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
	transform_.quaRot = Quaternion::Identity();

	// 注視点
	targetPos_ = AsoUtility::VECTOR_ZERO;

}

void Camera::SyncFollow(void)
{

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// Y軸
	rotY_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	// Y軸 + X軸
	transform_.quaRot = rotY_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// 注視点
	localPos = transform_.quaRot.PosAxis(FOLLOW_TARGET_LOCAL_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = transform_.quaRot.PosAxis(FOLLOW_CAMERA_LOCAL_POS);
	transform_.pos = VAdd(pos, localPos);

}

void Camera::ProcessRot(bool isLimit)
{
	// 方向回転によるXYZの移動(キーボード)
	RotKeyboard(isLimit);
	// 方向回転によるXYZの移動(ゲームパッド)
	RotGamePad(isLimit);
}

void Camera::ProcessMove(void)
{
}

//void Camera::SetBeforeDrawFixedPoint(void)
//{
//	// 何もしない
//}
//
//void Camera::SetBeforeDrawFree(void)
//{
//
//	// カメラ操作(回転)
//	ProcessRot(false);
//
//	// カメラ操作(移動)
//	ProcessMove();
//
//	// Y軸
//	rotY_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);
//
//	// Y軸 + X軸
//	transform_.quaRot = rotY_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));
//
//	// 注視点更新
//	targetPos_ = VAdd(transform_.pos, transform_.quaRot.PosAxis(FOLLOW_TARGET_LOCAL_POS));
//
//}

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
	VECTOR start = MV1GetFramePosition(followTransform_->modelId, 1);

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
		VECTOR dirToTarget = VNorm(VSub(targetPos_, transform_.pos));

		// 衝突点の少し手前にカメラを置く
		transform_.pos =
			VAdd(hitPoly.HitPosition, VScale(dirToTarget, COLLISION_BACK_DIS));

#pragma region 球体の衝突で線分衝突の補助として押し戻す

		// カメラ位置の球体コライダ
		int typeSphere = static_cast<int>(COLLIDER_TYPE::SPHERE);
		// 球体コライダがなければ処理を抜ける
		if (ownColliders_.count(typeSphere) == 0) continue;

		//// 球体コライダ情報
		//ColliderSphere* colliderSpehre =
		//	dynamic_cast<ColliderSphere*>(ownColliders_.at(typeSphere));
		//if (colliderSpehre == nullptr) return;

		// 指定された回数と距離で三角形の法線方向に押し戻す
		transform_.pos =
			ownColliders_.at(typeSphere)->GetPosPushBackAlongNormal(
				hitPoly, CNT_TRY_COLLISION_CAMERA, COLLISION_BACK_DIS);

#pragma endregion
	}
}

void Camera::RotKeyboard(bool isLimit)
{
	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH);
	const int centerX = screenW / 2;
	const int centerY = screenH / 2;

	const float SENS_MIN = 0.0002f;
	const float SENS_MAX = 0.0006f;
	const float SENSITIVITY = SENS_MIN + (SENS_MAX - SENS_MIN);

	// 現在のマウス座標を取得
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// 画面中心からの移動量を計算
	int deltaX = mouseX - centerX;
	int deltaY = mouseY - centerY;

	// 移動量に感度を掛けて、カメラの角度に足し込む
	angles_.y += deltaX * SENSITIVITY; // 左右回転
	angles_.x += deltaY * SENSITIVITY; // 上下回転

	// 角度制限（上下を見すぎないようにする）
	if (isLimit)
	{
		if (angles_.x < -LIMIT_X_DW_RAD) angles_.x = -LIMIT_X_DW_RAD;
		if (angles_.x > LIMIT_X_UP_RAD) angles_.x = LIMIT_X_UP_RAD;
	}

	// マウスカーソルを画面の中心に戻す（端にぶつからないようにするため）
	SetMousePoint(centerX, centerY);
}

void Camera::RotGamePad(bool isLimit)
{
	auto& ins = Input::GetInstance();

	// 右スティックの傾き
	VECTOR dir = ins.GetInstance().GetRightStickDirection();

	// 右スティック左右の傾き
	angles_.y += dir.x * ROT_POW_RAD;

	// 右スティック上下の傾き
	angles_.x -= dir.z * ROT_POW_RAD;

	// 角度制限
	if (isLimit && angles_.x < -LIMIT_X_DW_RAD)
	{
		angles_.x = -LIMIT_X_DW_RAD;
	}
	if (isLimit && angles_.x > LIMIT_X_UP_RAD)
	{
		angles_.x = LIMIT_X_UP_RAD;
	}
}
