#include <memory>
#include "../../Engine/Scene/SceneManager.h"
#include "TitleScene.h"

// 最初のシーンを生成する（宣言は SceneManager.h）
std::unique_ptr<SceneBase> CreateFirstScene(void)
{
	return std::make_unique<TitleScene>();
}