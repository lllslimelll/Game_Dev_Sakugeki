#pragma once
#include <unordered_map>
#include <vector>
#include <Dxlib.h>
#include "../Math/Vector2.h"

class Input
{

public:

	// 入力命令コマンド
	enum class Command
	{
		// アクション
		MOVE_FORWARD,// 前進
		MOVE_BACK,   // 後退
		MOVE_LEFT,   // 左移動
		MOVE_RIGHT,  // 右移動

		JUMP, // ジャンプ

		// システム
		DECIDE,	// 決定
		CLICK,  // クリック
		CANCEL, // キャンセル
		UP,		// 上
		DOWN,	// 下
		LEFT,	// 左
		RIGHT,	// 右
		PAUSE,	// ポーズ

		MAX
	};

	// インスタンスを明示的に生成
	static void CreateInstance(void);
	// インスタンスの取得
	static Input& GetInstance(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// リソースの破棄
	void Destroy(void);

	// 入力状態の取得
	// 押されているか
	bool IsPressed(Command cmd) const;
	// 押されたか
	bool IsTriggered(Command cmd) const;
	// 離されたか
	bool IsReleased(Command cmd) const;

	// マウス座標の取得
	Vector2 GetMousePos(void) const;
	// マウスホイールの回転量を取得
	int GetMouseWheelRot(void) const;

	// 左スティックの方向を取得（移動用）
	Vector2 GetLeftStickDirection(void) const;
	// 右スティックの方向を取得（視点用）
	Vector2 GetRightStickDirection(void) const;

	// 移動入力を取得
	Vector2 GetMoveInput(void) const;

private:
	
	// 周辺機器種別
	enum class PeripheralType
	{
		KEYBOARD,	// キーボード
		PAD,		// パッド
		MOUSE		// マウス
	};

	// 入力状態
	struct InputState
	{
		PeripheralType type;	// 周辺機器種別
		unsigned int id;		// 実入力の値
	};


	// コントローラーの認識番号
	enum class PAD_NO
	{
		KEY_PAD1,        // キー入力とパッド１入力
		PAD1,            // パッド１入力
		PAD2,            // パッド２入力
		PAD3,            // パッド３入力
		PAD4,            // パッド４入力
		INPUT_KEY = 4096 // キー入力
	};

	// コントローラーの種別
	enum class PAD_TYPE
	{
		OTHER = 0,
		XBOX_360,
		XBOX_ONE,
		DUAL_SHOCK_4,
		DUAL_SENSE,
		MAX
	};

	// コントローラーのボタン
	enum class PAD_BUTTON
	{
		// フェイスボタン
		TOP,	 // Xbox:Y / PS4:△
		BOTTOM,  // Xbox:A / PS4:×
		LEFT,    // Xbox:X / PS4:□
		RIGHT,   // Xbox:B / PS4:〇

		// ショルダー
		L_SHOULDER,   // Xbox:LB / PS4:L1
		R_SHOULDER,   // Xbox:RB / PS4:R1
		L_TRIGGER,    // Xbox:LT / PS4:L2
		R_TRIGGER,    // Xbox:RT / PS4:R2

		// スティック
		L_THUMB,      // 左スティック押し込み
		R_THUMB,      // 右スティック押し込み

		// 方向ボタン
		DIR_UP,    // 上
		DIR_DOWN,  // 下
		DIR_LEFT,  // 左
		DIR_RIGHT, // 右

		// スタート
		START,
	};

	// スティックの入力情報
	struct STICK_STATE
	{
		Vector2 left;  // 左スティック
		Vector2 right; // 右スティック
	};

	// シングルトン用静的インスタンス
	static Input* instance_;

	// 入力コマンドと入力デバイスの対応付けテーブル
	using InputTable_t = std::unordered_map<Command, std::vector<InputState>>;
	InputTable_t inputTable_;

	// 押されたかどうか記録用
	std::unordered_map<Command, bool> currentInputInfo_;
	std::unordered_map<Command, bool> lastInputInfo_;

	// マウス座標
	Vector2 mousePos_;	
	// ホイール回転量
	int mouseWheelRot_; 

	// スティック状態
	STICK_STATE stickState_;

	// アナログキーの入力受付しきい値(0.0～1.0)
	static constexpr float THRESHOLD = 0.35f;

	// コントローラー情報
	DINPUT_JOYSTATE dInState_;
	XINPUT_STATE    xInState_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Input(void);
	// コピーコンストラクタも同様
	Input(const Input& instance) = default;
	// デストラクタも同様
	~Input(void) = default;

	// 入力コマンドとデバイスの対応付けを設定
	void SetUpBindings(void);

	// コントローラーのボタンが押されているか判定する
	bool IsPadButtonActive(PAD_BUTTON button, PAD_TYPE type,
		const XINPUT_STATE& xState, const DINPUT_JOYSTATE& dState) const;

	// スティックの生の入力値にデッドゾーンを適用する
	Vector2 ApplyDeadZone(const Vector2& rawStick) const;

	// コントローラーの種別を取得する
	PAD_TYPE GetPadType(PAD_NO padNo) const;

	// DirectInput形式でコントローラーの入力情報を取得する
	DINPUT_JOYSTATE GetPadDInputState(PAD_NO padNo);

	// XInput形式でコントローラーの入力情報を取得する（XBOX用）
	XINPUT_STATE GetPadXInputState(PAD_NO padNo);

	// コントローラーの入力情報を取得してStickStateに変換する
	STICK_STATE GetStickState(PAD_NO padNo);

	// スティック状態を更新する
	void UpdateStickState(PAD_NO padNo);
};