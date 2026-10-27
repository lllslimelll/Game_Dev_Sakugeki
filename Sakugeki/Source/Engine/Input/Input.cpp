#include <array>
#include "../Core/Application.h"
//#include "../Utility/AsoUtility.h"
#include "Input.h"

Input* Input::instance_ = nullptr;

void Input::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Input();
	}
	instance_->Init();
}

Input& Input::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		Input::CreateInstance();
	}
	return *instance_;
}

void Input::Init(void)
{
	// 入力コマンドとデバイスの対応付けを設定
	SetUpBindings();

	// コマンドごとの入力状態を初期化
	for (auto& [cmd, state] : inputTable_)
	{
		currentInputInfo_[cmd] = false;
		lastInputInfo_[cmd] = false;
	}
}

void Input::Update(void)
{
	// マウスの座標とホイール回転量を取得
	GetMousePoint(&mousePos_.x, &mousePos_.y);
	mouseWheelRot_ = GetMouseWheelRotVol();

	// スティック状態の更新
	UpdateStickState(PAD_NO::PAD1);

	// 前フレームの入力状態を保存
	lastInputInfo_ = currentInputInfo_;

	// 入力デバイスの状態を取得
	std::array<char, 256> keyState;
	GetHitKeyStateAll(keyState.data());
	int             mouseState = GetMouseInput();

	XINPUT_STATE    xState = GetPadXInputState(PAD_NO::PAD1); // XInput形式
	DINPUT_JOYSTATE dState = GetPadDInputState(PAD_NO::PAD1); // DirectInput形式
	PAD_TYPE     padType = GetPadType(PAD_NO::PAD1); // コントローラーの種別

	// コマンドごとに入力状態を判定
	for (auto& [cmd, states] : inputTable_)
	{
		currentInputInfo_[cmd] = false;

		for (const auto& state : states)
		{
			// 入力デバイスごとに対応するボタンの入力状態を確認
			switch (state.type)
			{
			case PeripheralType::KEYBOARD:
				currentInputInfo_[cmd] = keyState[state.id];
				break;
			case PeripheralType::MOUSE:
				currentInputInfo_[cmd] = mouseState & state.id;
				break;
			case PeripheralType::PAD:
				currentInputInfo_[cmd] = IsPadButtonActive(
					static_cast<PAD_BUTTON>(state.id), padType, xState, dState);
				break;
			}

			if (currentInputInfo_[cmd])
			{
				break;
			}
		}
	}
}

void Input::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}

bool Input::IsPressed(Command cmd) const
{
	if (!currentInputInfo_.contains(cmd)) return false;
	return currentInputInfo_.at(cmd);
}

bool Input::IsTriggered(Command cmd) const
{
	if (!currentInputInfo_.contains(cmd)) return false;
	return currentInputInfo_.at(cmd) && !lastInputInfo_.at(cmd);
}

bool Input::IsReleased(Command cmd) const
{
	if (!currentInputInfo_.contains(cmd)) return false;
	return !currentInputInfo_.at(cmd) && lastInputInfo_.at(cmd);
}

Vector2 Input::GetMousePos(void) const
{
	return mousePos_;
}

Input::Input(void)
	:
	mousePos_(),
	dInState_(),
	xInState_()
{
}

Input::PAD_TYPE Input::GetPadType(PAD_NO padNo) const
{
	return static_cast<Input::PAD_TYPE>(GetJoypadType(static_cast<int>(padNo)));
}

DINPUT_JOYSTATE Input::GetPadDInputState(PAD_NO padNo)
{
	// コントローラ情報
	GetJoypadDirectInputState(static_cast<int>(padNo), &dInState_);
	return dInState_;
}

XINPUT_STATE Input::GetPadXInputState(PAD_NO padNo)
{
	// コントローラ情報
	GetJoypadXInputState(static_cast<int>(padNo), &xInState_);
	return xInState_;
}

Input::STICK_STATE Input::GetStickState(PAD_NO padNo)
{
	STICK_STATE ret = {};

	auto type = GetPadType(padNo);

	switch (type)
	{
	case PAD_TYPE::XBOX_360:
	case PAD_TYPE::XBOX_ONE:
	{
		// XInput形式でコントローラーの入力情報を取得
		auto x = GetPadXInputState(padNo);

		// XInputの値域（-32768?32767）を-1.0?1.0に正規化
		ret.leftX  = x.ThumbLX / 32767.0f;  // 左スティックX軸
		ret.leftY  = -x.ThumbLY / 32767.0f;  // 左スティックY軸
		ret.rightX = x.ThumbRX / 32767.0f;  // 右スティックX軸
		ret.rightY = -x.ThumbRY / 32767.0f;  // 右スティックY軸
		break;
	}
	case PAD_TYPE::DUAL_SHOCK_4:
	case PAD_TYPE::DUAL_SENSE:
	{
		// DirectInput形式でコントローラーの入力情報を取得
		auto d = GetPadDInputState(padNo);

		// DirectInputの値域（-1000?1000）を-1.0?1.0に正規化
		ret.leftX  = d.X / 1000.0f;	// 左スティックX軸
		ret.leftY  = -d.Y / 1000.0f;	// 左スティックY軸
		ret.rightX = d.Z / 1000.0f;	// 右スティックX軸
		ret.rightY = -d.Rz / 1000.0f;	// 右スティックY軸
		break;
	}
	default:
		break;
	}

	return ret;
}

void Input::UpdateStickState(PAD_NO padNo)
{
	// スティック状態を更新
	stickState_ = GetStickState(padNo);
}

int Input::GetMouseWheelRot(void) const
{
	return mouseWheelRot_;
}

VECTOR Input::GetLeftStickDirection(void) const
{
	return GetDirectionXZAKey(stickState_.leftX, stickState_.leftY);
}

VECTOR Input::GetRightStickDirection(void) const
{
	return GetDirectionXZAKey(stickState_.rightX, stickState_.rightY);
}

void Input::SetUpBindings(void)
{
	// 移動
	inputTable_[Command::MOVE_FORWARD] =
	{
		{ PeripheralType::KEYBOARD, KEY_INPUT_W },
	};
	inputTable_[Command::MOVE_BACK] =
	{
		{ PeripheralType::KEYBOARD, KEY_INPUT_S },
	};
	inputTable_[Command::MOVE_LEFT] =
	{
		{ PeripheralType::KEYBOARD, KEY_INPUT_A },
	};
	inputTable_[Command::MOVE_RIGHT] =
	{
		{ PeripheralType::KEYBOARD, KEY_INPUT_D },
	};

	inputTable_[Command::JUMP] =
	{
		{ PeripheralType::KEYBOARD, KEY_INPUT_SPACE },
		{ PeripheralType::PAD,	static_cast<int>(PAD_BUTTON::BOTTOM) }
	};

	// システム
	inputTable_[Command::DECIDE] =
	{
		{ PeripheralType::PAD,	static_cast<int>(PAD_BUTTON::BOTTOM) },
	};
	inputTable_[Command::CLICK] =
	{
		{ PeripheralType::MOUSE, MOUSE_INPUT_LEFT },  
	};
	inputTable_[Command::CANCEL] =
	{
		{ PeripheralType::PAD,	static_cast<int>(PAD_BUTTON::RIGHT) },
	};
	inputTable_[Command::UP] =
	{
		{ PeripheralType::PAD,	static_cast<int>(PAD_BUTTON::DIR_UP) },
	};
	inputTable_[Command::DOWN] =
	{
		{ PeripheralType::PAD,	static_cast<int>(PAD_BUTTON::DIR_DOWN) },
	};
	inputTable_[Command::LEFT] =
	{
		{ PeripheralType::PAD,	static_cast<int>(PAD_BUTTON::DIR_LEFT) },
	};
	inputTable_[Command::RIGHT] =
	{
		{ PeripheralType::PAD,	static_cast<int>(PAD_BUTTON::DIR_RIGHT) },
	};
	inputTable_[Command::PAUSE] =
	{
		{ PeripheralType::KEYBOARD, KEY_INPUT_ESCAPE },
		{ PeripheralType::PAD,      static_cast<int>(PAD_BUTTON::START) },
	};
}

bool Input::IsPadButtonActive(PAD_BUTTON button, PAD_TYPE type, const XINPUT_STATE& xState, const DINPUT_JOYSTATE& dState) const
{
	switch (type)
	{
	case PAD_TYPE::XBOX_360:
	case PAD_TYPE::XBOX_ONE:
	{
		//   Y
		// X   B
		//   A

		switch (button)
		{
		case PAD_BUTTON::TOP:        return xState.Buttons[XINPUT_BUTTON_Y] != 0;   // Y
		case PAD_BUTTON::BOTTOM:     return xState.Buttons[XINPUT_BUTTON_A] != 0;   // A
		case PAD_BUTTON::RIGHT:      return xState.Buttons[XINPUT_BUTTON_B] != 0;   // B
		case PAD_BUTTON::LEFT:       return xState.Buttons[XINPUT_BUTTON_X] != 0;   // X
		case PAD_BUTTON::L_SHOULDER: return xState.Buttons[XINPUT_BUTTON_LEFT_SHOULDER] != 0;  // LB
		case PAD_BUTTON::R_SHOULDER: return xState.Buttons[XINPUT_BUTTON_RIGHT_SHOULDER] != 0; // RB
		case PAD_BUTTON::L_TRIGGER:  return xState.LeftTrigger  > 128;	// LT
		case PAD_BUTTON::R_TRIGGER:  return xState.RightTrigger > 128;	// RT
		case PAD_BUTTON::L_THUMB:    return xState.Buttons[XINPUT_BUTTON_LEFT_THUMB] != 0;	// 左スティック押し込み
		case PAD_BUTTON::R_THUMB:    return xState.Buttons[XINPUT_BUTTON_RIGHT_THUMB] != 0;	// 右スティック押し込み
		case PAD_BUTTON::START:      return xState.Buttons[XINPUT_BUTTON_START] != 0;		// START
		case PAD_BUTTON::DIR_UP:     return xState.Buttons[XINPUT_BUTTON_DPAD_UP] != 0;		// 十字キー上
		case PAD_BUTTON::DIR_DOWN:   return xState.Buttons[XINPUT_BUTTON_DPAD_DOWN] != 0;	// 十字キー下
		case PAD_BUTTON::DIR_LEFT:   return xState.Buttons[XINPUT_BUTTON_DPAD_LEFT] != 0;	// 十字キー左
		case PAD_BUTTON::DIR_RIGHT:  return xState.Buttons[XINPUT_BUTTON_DPAD_RIGHT] != 0;	// 十字キー右
		default: 
			return false;
		}
	}
	case PAD_TYPE::DUAL_SHOCK_4:
	case PAD_TYPE::DUAL_SENSE:
	{
		//   △
		// □  〇
		//   ×

		switch (button)
		{
		case PAD_BUTTON::TOP:        return dState.Buttons[3] != 0;  // △
		case PAD_BUTTON::BOTTOM:     return dState.Buttons[1] != 0;  // ×
		case PAD_BUTTON::LEFT:       return dState.Buttons[0] != 0;  // □
		case PAD_BUTTON::RIGHT:      return dState.Buttons[2] != 0;  // 〇
		case PAD_BUTTON::L_SHOULDER: return dState.Buttons[4] != 0;  // L1
		case PAD_BUTTON::R_SHOULDER: return dState.Buttons[5] != 0;  // R1
		case PAD_BUTTON::L_TRIGGER:  return dState.Buttons[6] != 0;  // L2
		case PAD_BUTTON::R_TRIGGER:  return dState.Buttons[7] != 0;  // R2
		case PAD_BUTTON::L_THUMB:    return dState.Buttons[10] != 0; // 左スティック押し込み
		case PAD_BUTTON::R_THUMB:    return dState.Buttons[11] != 0; // 右スティック押し込み
		case PAD_BUTTON::START:      return dState.Buttons[9] != 0;  // START
		case PAD_BUTTON::DIR_UP:	 return dState.POV[0] == 0;      // 十字キー上（0度）
		case PAD_BUTTON::DIR_DOWN:	 return dState.POV[0] == 18000;  // 十字キー下（180度）
		case PAD_BUTTON::DIR_LEFT:	 return dState.POV[0] == 27000;  // 十字キー左（270度）
		case PAD_BUTTON::DIR_RIGHT:	 return dState.POV[0] == 9000;   // 十字キー右（90度）
		default: 
			return false;
		}
	}
	default:
		return false;
	}
}

// アナログキーの入力値から方向を取得
VECTOR Input::GetDirectionXZAKey(float aKeyX, float aKeyY) const
{
	VECTOR ret = { 0.0f, 0.0f, 0.0f };

	// スティックの入力値
	float dirX = aKeyX; 
	float dirZ = aKeyY;

	// 円形デッドゾーン
	float len = sqrtf(dirX * dirX + dirZ * dirZ);
	if (len < THRESHOLD) { return ret; }

	// デッドゾーン境界からの再スケーリング（可変デッドゾーン）
	float scale = (len - THRESHOLD) / (1.0f - THRESHOLD);
	dirX = (dirX / len) * scale;
	dirZ = (dirZ / len) * scale;

	// Zは前に倒すとマイナス値が返ってくるので反転
	ret = VNorm({ dirX, 0.0f, -dirZ });

	return ret;
}

