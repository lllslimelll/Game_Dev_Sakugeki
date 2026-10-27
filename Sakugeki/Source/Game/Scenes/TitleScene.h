#pragma once
#include <memory>
#include "SceneBase.h"

class Menu;

class TitleScene : public SceneBase
{
public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	// ロード
	void Load(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

private:

	// メニュー項目
	enum class MENU
	{
		GAME_START,
		OPTION,
		QUIT_GAME,
		MAX
	};

	void DrawTitle(void)  const;
	void DrawMenu(void)   const;

	// メニュー
	std::unique_ptr<Menu> menu_;
};
