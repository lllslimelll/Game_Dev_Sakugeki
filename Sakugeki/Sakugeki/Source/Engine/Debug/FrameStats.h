#pragma once
#include <array>
#include <chrono>

// 1フレームの処理時間を測る（時間はすべてミリ秒）
class FrameStats
{
public:

	// 記録するフレーム数（60FPS で 2 秒分）
	static constexpr int HISTORY_SIZE = 120;

	FrameStats(void);
	~FrameStats(void);

	// フレームの最初。ここで前のフレームの全体の時間が決まる
	void BeginFrame(void);
	void EndUpdate(void);
	void EndDraw(void);
	void EndDebug(void);
	void EndFlip(void);

	// 前のフレームの値
	float GetFrameMs(void) const;	// 全体（FPS 制御の待ちを含む）
	float GetUpdateMs(void) const;
	float GetDrawMs(void) const;
	float GetDebugMs(void) const;	// デバッグ画面を描く時間
	float GetFlipMs(void) const;	// ScreenFlip（垂直同期の待ち）

	// 直近 HISTORY_SIZE フレームから
	float GetAverageFrameMs(void) const;
	float GetMaxFrameMs(void) const;
	float GetFps(void) const;

	// 全体の時間の記録（リングバッファ）と、いちばん古い記録の位置
	const std::array<float, HISTORY_SIZE>& GetHistory(void) const;
	int GetHistoryHead(void) const;

private:

	std::chrono::steady_clock::time_point frameStart_;
	std::chrono::steady_clock::time_point lastMark_;
	bool hasFrame_;		// 最初のフレームには「前のフレーム」がない

	float frameMs_;
	float updateMs_;
	float drawMs_;
	float debugMs_;
	float flipMs_;

	std::array<float, HISTORY_SIZE> history_;
	int head_;			// 次に書き込む位置（＝いちばん古い記録）
	int count_;			// 記録した数（HISTORY_SIZE まで）

	// 前の区切りから今までの時間を返し、区切りを今にする
	float MarkMs(void);

	static float ToMs(std::chrono::steady_clock::time_point from, std::chrono::steady_clock::time_point to);
};