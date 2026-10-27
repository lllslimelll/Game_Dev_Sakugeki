#include <DxLib.h>
#include "../../Engine/Input/Input.h"
#include "../../Engine/Audio/Audio.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../Actor/Charactor/Player.h"
#include "../Actor/Stage/StageManager.h"
#include "../Actor/Camera.h"
#include "GameScene.h"

GameScene::GameScene(void)
	:
	SceneBase()
{
}

GameScene::~GameScene(void)
{
	// コライダの解放
	if (player_) { player_->Release(); }
	if (stageMng_) { stageMng_->Release(); }
}

void GameScene::Load()
{
	stageMng_ = std::make_unique<StageManager>();
	stageMng_->Load();

	player_ = std::make_unique<Player>();
	player_->Load();
}

void GameScene::Init(void)
{
	SetMouseDispFlag(false);

	// カメラ
	auto& camera = sceMng_.GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW);
	camera.SetFollow(&player_->GetTransform());

	// 初期化
	stageMng_->Init();
	player_->Init();

	// ステージ側の全コライダをキャラに登録
	for (const ColliderBase* col : stageMng_->GetColliders())
	{
		player_->AddHitCollider(col);
	}
}

void GameScene::Update(void)
{
	stageMng_->Update();
	player_->Update();
	sceMng_.GetCamera().Update();

	// シーン遷移
	auto const& ins = Input::GetInstance();
	if (ins.IsTriggered(Input::Command::PAUSE))
	{
		sceMng_.PushOverlay(SceneManager::SCENE_ID::PAUSE);
	}
}

void GameScene::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, L"ゲーム");

	stageMng_->Draw();
	player_->Draw();

	// 影描画
	//DrawShadow();
}

// リアルシャドウ描画
void GameScene::DrawShadow(void)
{
	const std::vector<int> stageModels = stageMng_->GetModelIds();
	// シャドウマップハンドルの作成
	int ShadowMapHandle = MakeShadowMap(1024, 1024);
	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(ShadowMapHandle, { 0.3f, -0.7f, 0.8f });
	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(ShadowMapHandle,
		VGet(player_->GetTransform().pos.x - 1000.0f, player_->GetTransform().pos.y - 1.0f, player_->GetTransform().pos.z - 1000.0f),
		VGet(player_->GetTransform().pos.x + 1000.0f, player_->GetTransform().pos.y + 1000.0f, player_->GetTransform().pos.z + 1000.0f));

	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(ShadowMapHandle);

	// シャドウマップへステージモデルの描画
	for (int model : stageModels) { MV1DrawModel(model); }
	// シャドウマップへキャラクターモデルの描画
	MV1DrawModel(player_->GetTransform().modelId);

	// シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, ShadowMapHandle);

	// ステージモデルの描画
	for (int model : stageModels) { MV1DrawModel(model); }
	// キャラクターモデルの描画
	MV1DrawModel(player_->GetTransform().modelId);

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);

	// シャドウマップの削除
	DeleteShadowMap(ShadowMapHandle);
}