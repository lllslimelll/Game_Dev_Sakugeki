#include "../../Engine/Common/Transform.h"
#include "ColliderModel.h"
#include "ColliderCapsule.h"


ColliderCapsule::ColliderCapsule(
    TAG tag, const Transform* follow,
    const Vector3& localPosTop, const Vector3& localPosDown, float radius)
    :
	ColliderBase(SHAPE::CAPSULE, tag, follow),
    localPosTop_(localPosTop),
	localPosDown_(localPosDown),
    radius_(radius)
{
}

ColliderCapsule::~ColliderCapsule(void)
{
}

const Vector3& ColliderCapsule::GetLocalPosTop(void) const
{
    return localPosTop_;
}

const Vector3& ColliderCapsule::GetLocalPosDown(void) const
{
    return localPosDown_;
}

void ColliderCapsule::SetLocalPosTop(const Vector3& pos)
{
    localPosTop_ = pos;
}

void ColliderCapsule::SetLocalPosDown(const Vector3& pos)
{
    localPosDown_ = pos;
}

Vector3 ColliderCapsule::GetPosTop(void) const
{
    return GetRotPos(localPosTop_);
}

Vector3 ColliderCapsule::GetPosDown(void) const
{
	return GetRotPos(localPosDown_);
}

float ColliderCapsule::GetRadius(void) const
{
    return radius_;
}

void ColliderCapsule::SetRadius(float radius)
{
    radius_ = radius;
}

float ColliderCapsule::GetHeight(void) const
{
    return localPosTop_.y;
}

Vector3 ColliderCapsule::GetCenter(void) const
{
    Vector3 top = GetPosTop();
    Vector3 down = GetPosDown();

    Vector3 diff = top - down;
    return down + (diff * 0.5f);
}

void ColliderCapsule::PushBackAlongNormal(
    const ColliderModel* colliderModel, Transform& transform,
    int maxTryCnt, float pushDistance, bool isExclude, bool isTarget) const
{
    // モデルとカプセルの衝突判定
    auto hits = MV1CollCheck_Capsule(
        colliderModel->GetFollow()->modelId, -1,
        GetPosTop().ToVECTOR(), GetPosDown().ToVECTOR(), GetRadius());

    // 衝突した複数のポリゴンと衝突回避するまで、位置を移動させる
    for (int i = 0; i < hits.HitNum; i++)
    {
        auto hitPoly = hits.Dim[i];

        // 除外フレームは無視する
        if (isExclude && colliderModel->IsExcludeFrame(hitPoly.FrameIndex)) continue;
        // 対象フレーム以外は無視する
        if (isTarget && colliderModel->IsTargetFrame(hitPoly.FrameIndex)) continue;

        // 指定された回数と距離で三角形の法線方向に押し戻す
        transform.pos =
            GetPosPushBackAlongNormal(hitPoly, maxTryCnt, pushDistance);
    }

    // 検出したポリゴン情報の後始末
    MV1CollResultPolyDimTerminate(hits);    
}

Vector3 ColliderCapsule::GetPosPushBackAlongNormal(
    const MV1_COLL_RESULT_POLY& hitCollPoly, int maxTryCnt, float pushDistance) const
{
    // コピー生成
    Transform tmpTransform = *follow_;
    ColliderCapsule tmpCapsule = *this;
    tmpCapsule.SetFollow(&tmpTransform);

    // 衝突補正処理
    int tryCnt = 0;
    while (tryCnt < maxTryCnt)
    {
        // カプセルと三角形の当たり判定
        if (!HitCheck_Capsule_Triangle(
            tmpCapsule.GetPosTop().ToVECTOR(), tmpCapsule.GetPosDown().ToVECTOR(), tmpCapsule.GetRadius(),
            hitCollPoly.Position[0], hitCollPoly.Position[1], hitCollPoly.Position[2]))
        {
            break;
        }

        // 衝突していたら法線方向に押し戻し
        tmpTransform.pos =
            tmpTransform.pos + (Vector3::GetVector3(hitCollPoly.Normal) * pushDistance);

        tryCnt++;
    }

    // 押し戻した座標を返す
    return tmpTransform.pos;
}

bool ColliderCapsule::IsHit(const ColliderModel* colliderModel, bool isExclude, bool isTarget) const
{
    bool ret = false;

    // モデルとカプセルの衝突判定
    auto hits = MV1CollCheck_Capsule(
        colliderModel->GetFollow()->modelId, -1,
        GetPosTop().ToVECTOR(), GetPosDown().ToVECTOR(), GetRadius());

    // 衝突した複数のポリゴンと衝突回避するまで、位置を移動させる
    for (int i = 0; i < hits.HitNum; i++)
    {
        auto hitPoly = hits.Dim[i];

        // 除外フレームは無視する
        if (isExclude && colliderModel->IsExcludeFrame(hitPoly.FrameIndex)) continue;
        // 対象フレーム以外は無視する
        if (isTarget && colliderModel->IsTargetFrame(hitPoly.FrameIndex)) continue;

       // 衝突している
        ret = true;
        break;
    }

    // 検出したポリゴン情報の後始末
    MV1CollResultPolyDimTerminate(hits);

    return ret;
}

void ColliderCapsule::DrawDebug(int color)
{
    // 上の球体
    Vector3 pos1 = GetPosTop();
    DrawSphere3D(pos1.ToVECTOR(), radius_, 5, color, color, false);

    // 下の球体
    Vector3 pos2 = GetPosDown();
    DrawSphere3D(pos2.ToVECTOR(), radius_, 5, color, color, false);

    Vector3 dir, start, end;

    // 球体を繋ぐ線(X+)
    dir = follow_->GetRight();
    start = pos1 + (dir * radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // 球体を繋ぐ線(X-)
    dir = follow_->GetLeft();
    start = pos1 + (dir *radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // 球体を繋ぐ線(Z+)
    dir = follow_->GetForward();
    start = pos1 + (dir *radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // 球体を繋ぐ線(Z-)
    dir = follow_->GetBack();
    start = pos1 + (dir *radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // カプセルの中心
    DrawSphere3D(GetCenter().ToVECTOR(), 5.0f, 10, color, color, true);
}
