#include <DxLib.h>
#include "Transform.h"

Transform::Transform(void)
	:
	scl(Vector3::ONE),
	rot(Vector3::ZERO),
	pos(Vector3::ZERO),
	localPos(Vector3::ZERO),
	matScl(Matrix3D::IDENTITY),
	matRot(Matrix3D::IDENTITY),
	matPos(Matrix3D::IDENTITY),
	quaRot(Quaternion::IDENTITY),
	quaRotLocal(Quaternion::IDENTITY)
{
}

Transform::~Transform(void)
{
}

void Transform::Update(void)
{
	// 大きさ
	matScl = Matrix3D::Scale(scl);

	// 回転
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix3D();

	// 位置
	matPos = Matrix3D::Translate(pos);
}

void Transform::Release(void)
{
}

Matrix3D Transform::GetMatrix(void) const
{
	// ワールド行列
	const Quaternion q = quaRot * quaRotLocal;

	// 拡縮 → 回転 → 平行移動
	return Matrix3D::Scale(scl) * q.ToMatrix3D() * Matrix3D::Translate(pos);
}

Vector3 Transform::GetForward(void) const
{
	return GetDir(Vector3::DIR_FORWARD);
}

Vector3 Transform::GetBack(void) const
{
	return GetDir(Vector3::DIR_BACK);
}

Vector3 Transform::GetRight(void) const
{
	return GetDir(Vector3::DIR_RIGHT);
}

Vector3 Transform::GetLeft(void) const
{
	return GetDir(Vector3::DIR_LEFT);
}

Vector3 Transform::GetUp(void) const
{
	return GetDir(Vector3::DIR_UP);
}

Vector3 Transform::GetDown(void) const
{
	return GetDir(Vector3::DIR_DOWN);
}

Vector3 Transform::GetDir(const Vector3& dir) const
{
	return quaRot.Rotate(dir);
}
