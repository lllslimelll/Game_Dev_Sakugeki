#pragma once
#include <DxLib.h>
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Math/Quaternion.h"

/// <summary>
/// モデル制御の基本情報
/// 大きさ：Vector3基準
/// 回転　：Quaternion基準
/// 位置　：Vector3基準
/// </summary>
class Transform
{
public:

	// コンストラクタ
	Transform(void);

	// デストラクタ
	~Transform(void);

	// モデル制御の基本情報更新
	void Update(void);

	// 解放
	void Release(void);

	// モデルのハンドルIDを設定
	void SetModel(int modelHId);

	// 前方方向を取得
	Vector3 GetForward(void) const;

	// 後方方向を取得
	Vector3 GetBack(void) const;

	// 右方向を取得
	Vector3 GetRight(void) const;

	// 左方向を取得
	Vector3 GetLeft(void) const;

	// 上方向を取得
	Vector3 GetUp(void) const;

	// 下方向を取得
	Vector3 GetDown(void) const;

	// 対象方向を取得
	Vector3 GetDir(const Vector3& dir) const;

	// モデルのハンドルID
	int modelId;

	// 大きさ
	Vector3 scl;
	// 回転
	Vector3 rot;
	// 位置
	Vector3 pos;
	Vector3 localPos;

	// 行列
	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// 回転
	Quaternion quaRot;

	// ローカル回転
	Quaternion quaRotLocal;
};
