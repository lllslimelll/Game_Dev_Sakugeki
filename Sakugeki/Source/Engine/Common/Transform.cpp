#include <DxLib.h>
#include "Transform.h"

Transform::Transform(void)
	:
	modelId(-1),
	scl(Vector3::ONE),
	rot(Vector3::ZERO),
	pos(Vector3::ZERO),
	localPos(Vector3::ZERO),
	matScl(MGetIdent()),
	matRot(MGetIdent()),
	matPos(MGetIdent()),
	quaRot(Quaternion()),
	quaRotLocal(Quaternion())
{
}

Transform::~Transform(void)
{
}

void Transform::Update(void)
{
	// 大きさ
	matScl = MGetScale(scl);

	// 回転
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// 位置
	matPos = MGetTranslate(pos);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// 行列をモデルに判定
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}
}

void Transform::Release(void)
{
}

void Transform::SetModel(int model)
{
	modelId = model;
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
	return quaRot.PosAxis(dir);
}
