#pragma once
#include <memory>

class SceneBase;
class UILoading;

// 非同期ロード画面
class AsyncLoadingScreen
{
public:

	AsyncLoadingScreen(void);
	~AsyncLoadingScreen(void);   // unique_ptr<UILoading> のため .cpp で定義

	// 画面用リソースの読み込み（同期）
	void Load(void);

	// 対象シーンのロードを非同期で発行
	void StartLoad(SceneBase& target);

	// 更新
	void Update(float deltaTime);

	// 描画
	void Draw(void) const;

	// ロードが完了し、演出も終わったか
	bool IsFinished(void) const;

private:

	// 描画
	std::unique_ptr<UILoading> ui_;

	// 発行した非同期ロードの数（進捗の分母）
	int total_;

	// 経過時間
	float time_;

	// 表示用の進捗（0〜1）
	float progress_;
};