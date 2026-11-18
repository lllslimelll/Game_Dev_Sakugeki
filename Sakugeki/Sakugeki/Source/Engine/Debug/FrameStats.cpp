#include <algorithm>
#include "FrameStats.h"

FrameStats::FrameStats(void)
	:
	frameStart_(),
	lastMark_(),
	hasFrame_(false),
	frameMs_(0.0f),
	updateMs_(0.0f),
	drawMs_(0.0f),
	debugMs_(0.0f),
	flipMs_(0.0f),
	history_(),
	head_(0),
	count_(0)
{
}

FrameStats::~FrameStats(void)
{
}

void FrameStats::BeginFrame(void)
{
	const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	// 前のフレームの全体の時間を記録する
	if (hasFrame_)
	{
		frameMs_ = ToMs(frameStart_, now);
		history_[head_] = frameMs_;
		head_ = (head_ + 1) % HISTORY_SIZE;
		count_ = (std::min)(count_ + 1, HISTORY_SIZE);
	}

	hasFrame_ = true;
	frameStart_ = now;
	lastMark_ = now;
}

void FrameStats::EndUpdate(void) { updateMs_ = MarkMs(); }
void FrameStats::EndDraw(void) { drawMs_ = MarkMs(); }
void FrameStats::EndDebug(void) { debugMs_ = MarkMs(); }
void FrameStats::EndFlip(void) { flipMs_ = MarkMs(); }

float FrameStats::GetFrameMs(void) const { return frameMs_; }
float FrameStats::GetUpdateMs(void) const { return updateMs_; }
float FrameStats::GetDrawMs(void) const { return drawMs_; }
float FrameStats::GetDebugMs(void) const { return debugMs_; }
float FrameStats::GetFlipMs(void) const { return flipMs_; }

float FrameStats::GetAverageFrameMs(void) const
{
	if (count_ == 0) { return 0.0f; }

	// 記録が埋まるまでは、0 から count_ - 1 に入っている
	float sum = 0.0f;
	for (int i = 0; i < count_; i++)
	{
		sum += history_[i];
	}
	return sum / count_;
}

float FrameStats::GetMaxFrameMs(void) const
{
	if (count_ == 0) { return 0.0f; }
	return *std::max_element(history_.begin(), history_.begin() + count_);
}

float FrameStats::GetFps(void) const
{
	const float average = GetAverageFrameMs();
	if (average <= 0.0f) { return 0.0f; }
	return 1000.0f / average;
}

const std::array<float, FrameStats::HISTORY_SIZE>& FrameStats::GetHistory(void) const
{
	return history_;
}

int FrameStats::GetHistoryHead(void) const
{
	return head_;
}

float FrameStats::MarkMs(void)
{
	const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	const float ms = ToMs(lastMark_, now);
	lastMark_ = now;
	return ms;
}

float FrameStats::ToMs(std::chrono::steady_clock::time_point from, std::chrono::steady_clock::time_point to)
{
	return std::chrono::duration<float, std::milli>(to - from).count();
}