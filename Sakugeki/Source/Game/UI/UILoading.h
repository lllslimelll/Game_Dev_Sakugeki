#pragma once
class UILoading
{
public:

	// 画像などの読み込み（同期）
	void Load(void);

	// 描画
	// progress：表示用の進捗（0〜1）
	// time    ：経過時間（秒）。アニメーション用
	void Draw(float progress, float time) const;
};
