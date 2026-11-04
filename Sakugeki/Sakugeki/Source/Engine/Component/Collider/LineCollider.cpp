#include "../../../Engine/Utility/AsoUtility.h"
#include "../../../Engine/Common/Transform.h"
#include "ModelCollider.h"
#include "LineCollider.h"

LineCollider::LineCollider(TAG tag, const Transform* follow,
	const Vector3& localPosStart, const Vector3& localPosEnd)
	:
	Collider(SHAPE::LINE, tag, follow),
	localPosStart_(localPosStart),
	localPosEnd_(localPosEnd)
{
}

LineCollider::~LineCollider(void)
{
}

void LineCollider::SetLocalPosStart(const Vector3& pos)
{
	localPosStart_ = pos;
}

void LineCollider::SetLocalPosEnd(const Vector3& pos)
{
	localPosEnd_ = pos;
}

const Vector3& LineCollider::GetLocalPosStart(void) const
{
	return localPosStart_;
}

const Vector3& LineCollider::GetLocalPosEnd(void) const
{
	return localPosEnd_;
}

Vector3 LineCollider::GetPosStart(void) const
{
	return GetRotPos(localPosStart_);
}

Vector3 LineCollider::GetPosEnd(void) const
{
	return GetRotPos(localPosEnd_);
}

bool LineCollider::PushBackUp(
	const ModelCollider* colliderModel, Transform& transform,
	float pushDistance, bool isExclude, bool isTarget) const
{
	bool isJump = false;

	// ステージモデル(地面)との衝突
	auto hits = MV1CollCheck_LineDim(
		colliderModel->GetFollow()->modelId, -1, GetPosStart().ToVECTOR(), GetPosEnd().ToVECTOR());

	for (int i = 0; i < hits.HitNum; i++)
	{
		auto hit = hits.Dim[i];

		// 除外フレームは無視する
		if (isExclude && colliderModel->IsExcludeFrame(hit.FrameIndex)) continue;
		// 指定フレーム以外は無視する
		if (isTarget && !colliderModel->IsTargetFrame(hit.FrameIndex)) continue;

		// 衝突地点から、少し上に移動
		if (transform.pos.y < hit.HitPosition.y)
		{
			transform.pos =
				Vector3::GetVector3(hit.HitPosition) + (Vector3::DIR_UP * 2.0f);
		}

		isJump = true;
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

	return isJump;
}

void LineCollider::DrawDebug(int color)
{
	Vector3 start = GetPosStart();
	Vector3 end = GetPosEnd();

	// 線分を描画
	DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

	// 始点・終点を球体で補助表示
	DrawSphere3D(start.ToVECTOR(), RADIUS, DIV_NUM, color, color, true);
	DrawSphere3D(end.ToVECTOR(), RADIUS, DIV_NUM, color, color, true);
}