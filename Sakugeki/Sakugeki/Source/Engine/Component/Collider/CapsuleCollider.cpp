#include "../../../Engine/Common/Transform.h"
#include "MeshCollider.h"
#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider(
    GameObject& gameObject, CollisionManager& collisionMng, TAG tag,
    const Vector3& localPosTop, const Vector3& localPosDown, float radius)
    :
    Collider(gameObject, collisionMng, SHAPE::CAPSULE, tag),
    localPosTop_(localPosTop),
    localPosDown_(localPosDown),
    radius_(radius)
{
}

CapsuleCollider::~CapsuleCollider(void)
{
}

const Vector3& CapsuleCollider::GetLocalPosTop(void) const
{
    return localPosTop_;
}

const Vector3& CapsuleCollider::GetLocalPosDown(void) const
{
    return localPosDown_;
}

void CapsuleCollider::SetLocalPosTop(const Vector3& pos)
{
    localPosTop_ = pos;
}

void CapsuleCollider::SetLocalPosDown(const Vector3& pos)
{
    localPosDown_ = pos;
}

Vector3 CapsuleCollider::GetPosTop(void) const
{
    return GetRotPos(localPosTop_);
}

Vector3 CapsuleCollider::GetPosDown(void) const
{
	return GetRotPos(localPosDown_);
}

float CapsuleCollider::GetRadius(void) const
{
    return radius_;
}

void CapsuleCollider::SetRadius(float radius)
{
    radius_ = radius;
}

float CapsuleCollider::GetHeight(void) const
{
    return localPosTop_.y;
}

Vector3 CapsuleCollider::GetCenter(void) const
{
    Vector3 top = GetPosTop();
    Vector3 down = GetPosDown();

    Vector3 diff = top - down;
    return down + (diff * 0.5f);
}

void CapsuleCollider::PushBackAlongNormal(
    const MeshCollider* colliderModel, Transform& transform,
    int maxTryCnt, float pushDistance, bool isExclude, bool isTarget) const
{
    // モデルとカプセルの衝突判定
    auto hits = MV1CollCheck_Capsule(
        colliderModel->GetTransform().modelId, -1,
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

Vector3 CapsuleCollider::GetPosPushBackAlongNormal(
    const MV1_COLL_RESULT_POLY& hitPoly, int maxTryCnt, float pushDistance) const
{
    // 持ち主の回転を反映したオフセット
    const Vector3& basePos = GetTransform().pos;
    const Vector3 offsetTop = GetPosTop() - basePos;
    const Vector3 offsetDown = GetPosDown() - basePos;
    // 位置
    Vector3 pos = basePos;
    // 衝突したポリゴンの法線ベクトル
    const Vector3 normal = Vector3::GetVector3(hitPoly.Normal);

    // 衝突補正処理
    for (int tryCnt = 0; tryCnt < maxTryCnt; tryCnt++)
    {
        if (!HitCheck_Capsule_Triangle(
            (pos + offsetTop).ToVECTOR(), (pos + offsetDown).ToVECTOR(), radius_,
            hitPoly.Position[0], hitPoly.Position[1], hitPoly.Position[2]))
        {
            break;
        }

        // 法線方向に押し戻す
        pos = pos + (normal * pushDistance);
    }

    return pos;
}

bool CapsuleCollider::IsHit(const MeshCollider* colliderModel, bool isExclude, bool isTarget) const
{
    bool ret = false;

    // モデルとカプセルの衝突判定
    auto hits = MV1CollCheck_Capsule(
        colliderModel->GetTransform().modelId, -1,
        GetPosTop().ToVECTOR(), GetPosDown().ToVECTOR(), GetRadius());

    // 衝突しているポリゴンがあるか調べる
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

void CapsuleCollider::DrawDebug(int color)
{
    // 上の球体
    Vector3 pos1 = GetPosTop();
    DrawSphere3D(pos1.ToVECTOR(), radius_, 5, color, color, false);

    // 下の球体
    Vector3 pos2 = GetPosDown();
    DrawSphere3D(pos2.ToVECTOR(), radius_, 5, color, color, false);

    Vector3 dir, start, end;

    // 球体を繋ぐ線(X+)
    dir = GetTransform().GetRight();
    start = pos1 + (dir * radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // 球体を繋ぐ線(X-)
    dir = GetTransform().GetLeft();
    start = pos1 + (dir *radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // 球体を繋ぐ線(Z+)
    dir = GetTransform().GetForward();
    start = pos1 + (dir *radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // 球体を繋ぐ線(Z-)
    dir = GetTransform().GetBack();
    start = pos1 + (dir *radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // カプセルの中心
    DrawSphere3D(GetCenter().ToVECTOR(), 5.0f, 10, color, color, true);
}
