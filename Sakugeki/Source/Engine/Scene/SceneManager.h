#pragma once
#include <memory>
#include <vector>
#include <chrono>
#include <DxLib.h>

class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	// 背景色
	static constexpr int BACKGROUND_COLOR_R = 60;
	static constexpr int BACKGROUND_COLOR_G = 60;
	static constexpr int BACKGROUND_COLOR_B = 60;

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		PROLOGUE,
		GAME,
		PAUSE,
		OPTION,
		DEAD,
		RESULT,
		DEBUG,
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	// 初期化
	void Init(void);
	
	// 3Dの初期化
	void Init3D(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);
	// オーバーレイの追加（オーバーレイとして）
	void PushOverlay(SCENE_ID sceneId);
	// オーバーレイの削除
	void PopOverlay();

	// カメラの取得
	Camera& GetCamera(void);

	// シーンIDの取得
	SCENE_ID GetSceneID(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

private:

	// privateにして外部から生成できない様にする
	SceneManager(void);
	SceneManager(const SceneManager& instance) = default;
	~SceneManager(void) = default;

	// シーン遷移実行
	void DoChangeScene(SCENE_ID sceneId);

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// フェード
	void Fade(void);

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// 各種シーン
	std::vector<std::unique_ptr<SceneBase>> scenes_;

	// フェード
	std::unique_ptr<Fader> fader_;

	// カメラ
	std::unique_ptr<Camera> camera_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// 前フレームの時間
	std::chrono::system_clock::time_point preTime_;
	// デルタタイム
	float deltaTime_;
};