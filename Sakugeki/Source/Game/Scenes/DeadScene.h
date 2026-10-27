#pragma once
#include "SceneBase.h"

class DeadScene : public SceneBase
{
public:

    DeadScene();
    ~DeadScene() override;

    void Init() override;
    void Update(void) override;
    void Draw(void)   override;

private:

    enum class MENU
    {
        RETRY,
        TITLE,
        MAX,
    };

    MENU menu_;

    void UpdateMenu(void);
    void DrawMenu(void);
};

