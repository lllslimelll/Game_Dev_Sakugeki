#pragma once
#include "../../../Engine/GameObject/GameObject.h"

// 木（今は全部の木を1つのモデルとして一括ロード）
class StageTree : public GameObject
{
public:

	enum class COLLIDER_TYPE
	{
		MAX
	};

	StageTree(void);
	~StageTree(void) override;

	void Load(void) override;
	void Update(void) override;

protected:

	void InitTransform(void) override;
	void InitRenderer(void) override;
	void InitCollider(void) override;
	void InitAnimation(void) override {}
	void InitPost(void) override {}
};