#pragma once
#include <unordered_map>
#include <unordered_set>
#include <DxLib.h>
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Math/Quaternion.h"
#include "ActorBase.h"

class Transform;
class ColliderSphere;

class Camera : public ActorBase
{
public:

	// カメラの初期座標
	static inline Vector3 DERFAULT_POS = { 0.0f, 0.0f, -500.0f };
	
	// カメラの初期角度
	static inline Vector3 DERFAULT_ANGLES = Vector3::ZERO;

	// パッドでの回転速度（度/秒）
	static constexpr float PAD_ROT_SPEED_DEG = 120.0f;
	// マウスの回転感度（rad/ピクセル）
	static constexpr float MOUSE_SENSITIVITY = 0.0006f;

	// カメラの移動スピード
	static constexpr float SPEED = 20.0f;

	// カメラのクリップ範囲
	static constexpr float VIEW_NEAR = 10.0f;
	static constexpr float VIEW_FAR = 20000.0f;

	// 追従位置からカメラ位置までの相対座標
	static inline Vector3 FOLLOW_CAMERA_LOCAL_POS = { 0.0f, 200.0f, -300.0f };

	// 追従位置から注視点までの相対座標
	static inline Vector3 FOLLOW_TARGET_LOCAL_POS = { 0.0f, 0.0f, 0.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 10.0f * (DX_PI_F / 180.0f);
	
	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW,
	};

	// 衝突判定種別
	enum class COLLIDER_TYPE
	{
		SPHERE,
		MAX,
	};

	// コンストラクタ
	Camera(void);

	// デストラクタ
	~Camera(void) override;

	// 更新
	void Update(void) override;

	void UpdateFollow();

	// 描画前のカメラ設定
	void SetBeforeDraw(void);

	// デバッグ用描画
	void DrawDebug(void);

	// 解放
	void Release(void) override;

	// 座標の取得
	const Vector3& GetPos(void) const;

	// 角度の取得
	const Vector3& GetAngles(void) const;
	const Quaternion& GetQuaRot(void) const;
	
	// Y軸のみのカメラ角度
	const Quaternion& GetQuaRotY(void) const;
	
	// 注視点の取得
	const Vector3& GetTargetPos(void) const;

	// カメラの前方方向
	Vector3 GetForward(void) const;

	// カメラモードの変更
	void ChangeMode(MODE mode);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

private:

	void Load() override {};

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override {}

	// 描画設定の初期化
	void InitRenderer() override {};

	// 衝突判定の初期化
	void InitCollider(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override {}

	// 初期化後の個別処理
	void InitPost(void) override;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象との位置同期を取る
	void SyncFollow(void);

	// カメラ操作
	void ProcessRot(bool isLimit);

	// カメラ回転量(キーボード)
	Vector2 GetRotMouse();
	// カメラ回転量(ゲームパッド)
	Vector2 GetRotPad();

	// モード別更新ステップ
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);

	// 衝突判定
	void Collision(void);

	// 衝突時の押し戻し試行回数
	static constexpr int CNT_TRY_COLLISION_CAMERA = 30;

	// 衝突時の押し戻し量
	static constexpr float COLLISION_BACK_DIS = 2.0f;

	// 衝突判定用球体判定
	static constexpr float COL_CAPSULE_SPHERE = 50.0f;

	// カメラの補完移動率
	static constexpr float LERP_RATE_MOVE = 0.1f;

	// カメラの更新前位置
	Vector3 prePos_;

	// カメラが追従対象とするTransform
	const Transform* followTransform_;

	// カメラモード
	MODE mode_;

	// カメラ角度(rad)
	Vector3 angles_;

	// カメラ角度(Y軸のみ)
	Quaternion rotY_;

	// 注視点
	Vector3 targetPos_;
};
