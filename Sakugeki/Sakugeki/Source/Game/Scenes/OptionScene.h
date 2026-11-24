#pragma once
#include "../../Engine/Scene/SceneBase.h"

class OptionScene : public SceneBase
{
public:

    OptionScene(void);
    ~OptionScene(void) override;

    void Init(void)   override;
    void Update(void) override;
    void Draw(void)   override;

private:
};