#include <algorithm>
#include <DxLib.h>
#include "../Scenes/SceneBase.h"
#include "../UI/UILoading.h"
#include "AsyncLoadingScreen.h"

namespace
{
	// 最低表示時間
	constexpr float MIN_TIME = 1.5f;

	// 表示用の進捗が実際の進捗に追従する割合（1フレームあたり）
	constexpr float PROGRESS_FOLLOW = 0.05f;

	// 完了とみなす表示用の進捗
	constexpr float PROGRESS_FINISH = 0.99f;
}

AsyncLoadingScreen::AsyncLoadingScreen(void)
	:
	ui_(nullptr),
	total_(0),
	time_(0.0f),
	progress_(0.0f)
{
}

AsyncLoadingScreen::~AsyncLoadingScreen(void)
{
}

void AsyncLoadingScreen::Load(void)
{
	// 非同期にする前に、画面用のリソースを同期で読む
	ui_ = std::make_unique<UILoading>();
	ui_->Load();
}

void AsyncLoadingScreen::StartLoad(SceneBase& target)
{
	// 非同期ロード
	SetUseASyncLoadFlag(TRUE);  // 発行
	target.Load();
	SetUseASyncLoadFlag(FALSE); // 発行終了

	// 発行した数を進捗の分母として保存
	total_ = GetASyncLoadNum();
}

void AsyncLoadingScreen::Update(float deltaTime)
{
	time_ += deltaTime;

	// 実際の進捗（読み込むものがなければ完了済み）
	float actual = 1.0f;
	if (total_ > 0)
	{
		actual = 1.0f - static_cast<float>(GetASyncLoadNum()) / static_cast<float>(total_);
	}

	// 表示用の進捗は滑らかに追従させる（戻らない）
	float next = progress_ + (actual - progress_) * PROGRESS_FOLLOW;
	progress_ = max(progress_, next);
}

void AsyncLoadingScreen::Draw(void) const
{
	ui_->Draw(progress_, time_);
}

bool AsyncLoadingScreen::IsFinished(void) const
{
	if (total_ == 0) { return true; }

	// 非同期ロード終了と最低表示時間で終了
	return GetASyncLoadNum() == 0
		&& time_ >= MIN_TIME
		&& progress_ >= PROGRESS_FINISH;
}