#pragma once

class Character;
class Camera;

/// <summary>
/// プレイヤーの操作を行うクラス
/// 入力を読み取り、キャラクターの移動コンポーネントに指示する
/// </summary>
class PlayerController
{
public:

	/// <summary>
	/// コンストラクタ
	/// character : 操作するキャラクター
	/// camera    : 移動方向の基準にするカメラ
	/// </summary>
	PlayerController(Character& character, const Camera& camera);

	// デストラクタ
	~PlayerController(void);

	// 更新
	void Update(void);

private:

	// 操作するキャラクター
	Character& character_;

	// 移動方向の基準にするカメラ
	const Camera& camera_;
};
