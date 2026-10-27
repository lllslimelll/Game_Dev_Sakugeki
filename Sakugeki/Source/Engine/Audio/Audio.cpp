#include "Audio.h"
#include "../Input/Input.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ResourcePath.h"
#include "DxLib.h"

// グローバル領域で変数を初期化
Audio* Audio::instance_ = nullptr;

Audio::Audio(void)
{
}

//インスタンスの生成
void Audio::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Audio;
	}

	instance_->Init();
}

Audio& Audio::GetInstance(void)
{
	return *instance_;
}

void Audio::Load()
{
	auto& resMng = ResourceManager::GetInstance();

	bgmTitleH_ = resMng.Load(ResourceManager::SRC::BGM_TITLE).handleId_;
	bgmGameH_ = resMng.Load(ResourceManager::SRC::BGM_GAME).handleId_;
}

void Audio::Init(void)
{
	// 音量調整
	ChangeVolumeSoundMem(VOLUME_MAX, bgmTitleH_);
	ChangeVolumeSoundMem(VOLUME_MAX, bgmGameH_);
}

void Audio::Update(void)
{
}

void Audio::Destroy(void)
{
	DeleteSoundMem(bgmTitleH_);
	DeleteSoundMem(bgmGameH_);

	// インスタンスのメモリ解放
	delete instance_;
}

void Audio::PlayBGMTitle()
{
	// 再生
	if (CheckSoundMem(bgmTitleH_) == 0)
	{
		PlaySoundMem(bgmTitleH_, DX_PLAYTYPE_LOOP, true);
	}
}

void Audio::PlayBGMGame()
{
	// 再生
	if (CheckSoundMem(bgmGameH_) == 0)
	{
		PlaySoundMem(bgmGameH_, DX_PLAYTYPE_LOOP, true);
	}
}

void Audio::StopBGMTitle()
{
	StopSoundMem(bgmTitleH_);
}

void Audio::StopBGMGame()
{
	StopSoundMem(bgmGameH_);
}

void Audio::SetVolumeBGM(int vol)
{
	ChangeVolumeSoundMem(vol, bgmTitleH_);
	ChangeVolumeSoundMem(vol, bgmGameH_);
}

void Audio::SetVolumeSE(int vol)
{
}