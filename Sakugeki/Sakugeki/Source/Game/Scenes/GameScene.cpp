#include <algorithm>
#include <DxLib.h>
#include "../../Engine/Input/Input.h"
#include "../../Engine/Audio/Audio.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Math/UtilityMath.h"
#include "../../Engine/Component/Collider/CollisionManager.h"
#include "../../Engine/Core/Application.h"
#include "../../Engine/Debug/DebugSystem.h"
#include "../Actor/Charactor/Player.h"
#include "../Actor/Stage/StageManager.h"
#include "../Actor/Camera.h"
#include "../../Engine/Component/Health.h"
#include "../../Game/UI/Player/HPGauge.h"
#include "../../Game/Common/CollisionLayer.h"
#include "../../Game/Scenes/PauseScene.h"
#include "GameScene.h"

GameScene::GameScene()
	:
	SceneBase()
{
}

GameScene::~GameScene()
{
	if (hpGauge_) { hpGauge_->Release(); }
	if (player_) { player_->Release(); }
	if (stageMng_) { stageMng_->Release(); }
}

void GameScene::Load()
{
	// 当たり判定の管理クラス
	collisionMng_ = std::make_unique<CollisionManager>();

	camera_ = std::make_unique<Camera>(*collisionMng_);

	stageMng_ = std::make_unique<StageManager>(*collisionMng_);
	stageMng_->Load();

	player_ = std::make_unique<Player>(*camera_, *collisionMng_);
	player_->Load();

	// プレイヤーの HP ゲージ
	const Health* health = player_->GetComponent<Health>();
	hpGauge_ = std::make_unique<HPGauge>(*health);
}

void GameScene::Init(void)
{
	SetMouseDispFlag(false);

	// カメラ
	camera_->Init();
	camera_->ChangeMode(Camera::MODE::FOLLOW);
	camera_->SetFollow(&player_->GetTransform());

	// 初期化
	stageMng_->Init();
	player_->Init();
	hpGauge_->Init();

	CollisionLayer::SetupMatrix(*collisionMng_);
	CollisionLayer::RegisterDebugNames(Application::GetInstance().GetDebugSystem());
}

void GameScene::Update(void)
{
	stageMng_->Update();
	player_->Update();
	hpGauge_->Update(sceMng_.GetDeltaTime());
	camera_->Update();

	// 全員が動き終わった後に、コライダーの形をデバッグ表示へ渡す
	collisionMng_->AddDebugShapes();

	// シーン遷移
	auto const& ins = Input::GetInstance();
	if (ins.IsTriggered(Input::Command::PAUSE))
	{
		sceMng_.PushOverlay(std::make_unique<PauseScene>());
	}
}

void GameScene::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, L"ゲーム");

	camera_->SetBeforeDraw();

	stageMng_->Draw();
	player_->Draw();
	hpGauge_->Draw();

	// 影描画
	//DrawShadow();
}

// リアルシャドウ描画
void GameScene::DrawShadow(void)
{
	//const std::vector<int> stageeModels = stageMng_->GetModelIds();
	//// シャドウマップハンドルの作成
	//int ShadowMapHandle = MakeShadowMap(1024, 1024);
	//// シャドウマップが想定するライトの方向もセット
	//SetShadowMapLightDirection(ShadowMapHandle, { 0.3f, -0.7f, 0.8f });
	//// シャドウマップに描画する範囲を設定
	//SetShadowMapDrawArea(ShadowMapHandle,
	//	VGet(player_->GetTransform().pos.x - 1000.0f, player_->GetTransform().pos.y - 1.0f, player_->GetTransform().pos.z - 1000.0f),
	//	VGet(player_->GetTransform().pos.x + 1000.0f, player_->GetTransform().pos.y + 1000.0f, player_->GetTransform().pos.z + 1000.0f));

	//// シャドウマップへの描画の準備
	//ShadowMap_DrawSetup(ShadowMapHandle);

	//// シャドウマップへステージモデルの描画
	//for (int model : stageeModels) { MV1DrawModel(model); }
	//// シャドウマップへキャラクターモデルの描画
	//MV1DrawModel(player_->GetTransform().modelId);

	//// シャドウマップへの描画を終了
	//ShadowMap_DrawEnd();

	//// 描画に使用するシャドウマップを設定
	//SetUseShadowMap(0, ShadowMapHandle);

	//// ステージモデルの描画
	//for (int model : stageeModels) { MV1DrawModel(model); }
	//// キャラクターモデルの描画
	//MV1DrawModel(player_->GetTransform().modelId);

	//// 描画に使用するシャドウマップの設定を解除
	//SetUseShadowMap(0, -1);

	//// シャドウマップの削除
	//DeleteShadowMap(ShadowMapHandle);
}