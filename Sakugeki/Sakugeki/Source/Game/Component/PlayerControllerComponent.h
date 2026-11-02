#pragma once

class Character;
class Camera;

/// <summary>
/// プレイヤーの操作を行うクラス
/// 入力を読み取り、キャラクターの移動コンポーネントに指示する
/// </summary>
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
