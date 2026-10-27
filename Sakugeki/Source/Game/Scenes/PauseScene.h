#pragma once
#include <memory>
#include "SceneBase.h"

class Menu;

class PauseScene : public SceneBase
{
public:

	PauseScene();
	~PauseScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	std::unique_ptr<Menu> menu_;

	void UpdateMenu(void);
	void DrawMenu(void);
};