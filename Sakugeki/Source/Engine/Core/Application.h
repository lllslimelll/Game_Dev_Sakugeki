#pragma once
#include <memory>
#include <string>
class FpsController;

class Application
{

public:
	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1920;
	static constexpr int SCREEN_SIZE_Y = 1080;
	// スクリーンの色深度
	static constexpr int COLOR_BIT_DEPTH = 32;

	// 固定FPS
	static constexpr int FRAME_RATE = 60;

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);

	// ゲームループの開始
	void Run(void);

	// リソースの破棄
	void Destroy(void);

	// 初期化成功／失敗の判定
	bool IsInitFail(void) const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void) const;

	// 終了リクエスト
	bool RequestExit();

private:

	// 静的インスタンス
	static Application* instance_;

	// FPS制御
	std::unique_ptr<FpsController> fpsController_;

	// 終了リクエストフラグ
	bool isRequestedExit_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application(void);

	// コピーコンストラクタも同様
	Application(const Application& instance) = default;

	// デストラクタも同様
	~Application(void) = default;

	// エフェクシアの初期化
	void InitEffekseer(void);

};