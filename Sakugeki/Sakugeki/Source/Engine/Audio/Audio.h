#pragma once
#include <DxLib.h>
#include <string>

static const std::wstring PATH_BGM_TITLE = L"Assets/Sound/BGM_Title.mp3";
static const std::wstring PATH_BGM_GAME = L"Assets/Sound/BGM_Game.mp3";

class Audio
{
public:

	// 音量（０～２５５）
	static constexpr int VOLUME_MIN = 0;	// 最小音量
	static constexpr int VOLUME_MAX = 255;	// 最大音量

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static Audio& GetInstance(void);

	void Load();
	void Init(void);
	void Update(void);
	void Destroy(void);

	// 音源再生
	void PlayBGMTitle();
	void PlayBGMGame();

	//音源停止	
	void StopBGMTitle();		// タイトルBGM
	void StopBGMGame();

	void SetVolumeBGM(int vol);  
	void SetVolumeSE(int vol);   

private:

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Audio(void);

	// コピーコンストラクタも同様
	Audio(const Audio& instance) = default;

	// デストラクタも同様
	~Audio(void) = default;

	// 静的インスタンス
	static Audio* instance_;

	int bgmTitleH_;
	int bgmGameH_;
};