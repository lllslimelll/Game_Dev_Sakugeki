#pragma once

class Character;
class Camera;

// プレイヤーの操作を行うクラス
class PlayerControllerComponent
{
public:

	/// <summary>
	/// コンストラクタ
	/// character : 操作するキャラクター
	/// camera    : 移動方向の基準にするカメラ
	/// </summary>
	PlayerControllerComponent(Character& character, const Camera& camera);

	// デストラクタ
	~PlayerControllerComponent(void);

	// 更新
	void Update(void);

private:

	// 操作するキャラクター
	Character& character_;

	// 移動方向の基準にするカメラ
	const Camera& camera_;
};
