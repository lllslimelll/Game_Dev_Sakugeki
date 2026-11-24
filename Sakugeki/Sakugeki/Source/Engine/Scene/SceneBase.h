#pragma once
class ResourceManager;
class SceneManager;
class Input;

class SceneBase
{
public:

	// コンストラクタ
	SceneBase(void);

	// デストラクタ
	virtual ~SceneBase(void);

	// ロード
	virtual void Load(void) {};

	// 初期化
	virtual void Init(void) {};

	// 更新
	virtual void Update(void) = 0;

	// 描画
	virtual void Draw(void) = 0;

protected:

	// リソース管理
	ResourceManager& resMng_;
	// シーン管理
	SceneManager& sceMng_;
	// 入力管理
	Input& input_;
};
