#include <DxLib.h>
#include "../../Engine/Input/Input.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Core/Application.h"
#include "../../Engine/Debug/DebugSystem.h"
#include "../../Engine/GameObject/World.h"
#include "../../Engine/Component/Health.h"
#include "../Actor/Charactor/Player.h"
#include "../Actor/Stage/Ground.h"
#include "../Actor/Stage/Tree.h"
#include "../Actor/Camera.h"
#include "../UI/Player/HPGauge.h"
#include "../Common/CollisionLayer.h"
#include "PauseScene.h"
#include "GameScene.h"

GameScene::GameScene(void)
	:
	SceneBase(),
	world_(std::make_unique<World>()),
	camera_(nullptr),
	player_(nullptr),
	hpGauge_(nullptr)
{
	CollisionManager& collisionMng = world_->GetCollisionManager();

	// 3D 空間の中身（World に入れた順に、更新と描画が行われる）
	// カメラはプレイヤーより後に更新したい（プレイヤーが動いた後の位置を追うため）。
	// ただしプレイヤーを作るのにカメラが必要なので、作るのだけ先にして、入れるのは最後にする
	auto camera = std::make_unique<Camera>(collisionMng);
	camera_ = camera.get();

	world_->AddObject(std::make_unique<Ground>(collisionMng));
	world_->AddObject(std::make_unique<Tree>(collisionMng));

	auto player = std::make_unique<Player>(*camera_, collisionMng);
	player_ = player.get();
	world_->AddObject(std::move(player));

	world_->AddObject(std::move(camera));

	// UI
	hpGauge_ = std::make_unique<HPGauge>(*player_->GetComponent<Health>());
}

GameScene::~GameScene(void)
{
	if (hpGauge_) { hpGauge_->Release(); }
	if (world_) { world_->Release(); }
}

void GameScene::Load(void)
{
	// 全オブジェクトの素材の読み込みを発行する
	world_->Load();
}

void GameScene::Init(void)
{
	SetMouseDispFlag(false);

	// 衝突レイヤーの設定
	CollisionLayer::SetupMatrix(world_->GetCollisionManager());
	// デバッグ用のレイヤー名を登録
	CollisionLayer::RegisterDebugNames(Application::GetInstance().GetDebugSystem());

	// 全オブジェクトの初期化
	world_->Init();

	// カメラはプレイヤーを追従
	camera_->ChangeMode(Camera::MODE::FOLLOW);
	camera_->SetFollow(&player_->GetTransform());

	// UI
	hpGauge_->Init();
}

void GameScene::Update(void)
{
	world_->Update();
	hpGauge_->Update(sceMng_.GetDeltaTime());

	// ポーズ
	if (input_.IsTriggered(Input::Command::PAUSE))
	{
		sceMng_.PushOverlay(std::make_unique<PauseScene>());
	}
}

void GameScene::Draw(void)
{
	DrawFormatString(0, 0, 0xffffff, L"ゲーム");

	camera_->SetBeforeDraw();
	world_->Draw();
	hpGauge_->Draw();
}