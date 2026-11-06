#include <algorithm>
#include <DxLib.h>
#include "../../Engine/Input/Input.h"
#include "../../Engine/Audio/Audio.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Math/UtilityMath.h"
#include "../../Engine/Component/Collider/CollisionManager.h"
#include "../Actor/Charactor/Player.h"
#include "../Actor/Stage/StageManager.h"
#include "../Actor/Camera.h"
#include "../../Engine/Component/Health.h"
#include "../../Game/UI/Player/HPGauge.h"
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
	debugHud_.Release();
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

	debugHud_.Init();
	debugHudData_.hp = 100;
	debugHudData_.maxHp = 100;
	debugHudData_.spirit = 0.0f;
	debugHudData_.maxSpirit = 100.0f;
	debugHudData_.moves = {
		{ L"Y" }, { L"X+X" }, { L"B+X" }, { L"X長押し" }, { L"空中+X+X" }, { L"X+X+X長押し" },
	};
}

void GameScene::Update(void)
{
	stageMng_->Update();
	player_->Update();
	hpGauge_->Update(sceMng_.GetDeltaTime());
	camera_->Update();

	// シーン遷移
	auto const& ins = Input::GetInstance();
	if (ins.IsTriggered(Input::Command::PAUSE))
	{
		sceMng_.PushOverlay(SceneManager::SCENE_ID::PAUSE);
	}

	if (CheckHitKey(KEY_INPUT_F1)) player_->GetComponent<Health>()->TakeDamage(1);


	// TODO(仮UI) 動作確認用
	//   F1：ダメージ  F2(長押し)：加護ゲージ  F3：技を1つ達成  F4：技リストを戻す  F5：全回復
	HudData& hud = debugHudData_;

	if (IsDebugKeyTriggered(KEY_INPUT_F1)) { hud.hp = (std::max)(0, hud.hp - 15); }
	if (IsDebugKeyTriggered(KEY_INPUT_F5)) { hud.hp = hud.maxHp; }
	if (CheckHitKey(KEY_INPUT_F2))
	{
		hud.spirit = (std::min)(hud.maxSpirit, hud.spirit + 40.0f * sceMng_.GetDeltaTime());
	}
	if (IsDebugKeyTriggered(KEY_INPUT_F3))
	{
		for (auto& move : hud.moves)
		{
			if (!move.isDone) { move.isDone = true; break; }
		}
	}
	if (IsDebugKeyTriggered(KEY_INPUT_F4))
	{
		for (auto& move : hud.moves) { move.isDone = false; }
	}

	// 以下はゲームのルールの代わり（本来はゲージや技リストのクラスが決める）
	constexpr int HEAL_GUIDE_HPGauge = 50;   // これ以下なら回復を勧める
	hud.isBloom = !hud.moves.empty() &&
		std::all_of(hud.moves.begin(), hud.moves.end(), [](const HudMove& m) { return m.isDone; });

	if (hud.spirit < hud.maxSpirit)
	{
		hud.guide = HudData::GUIDE::NONE;
	}
	else
	{
		hud.guide = (hud.hp <= HEAL_GUIDE_HPGauge) ? HudData::GUIDE::HEAL : HudData::GUIDE::FINISHER;
	}
	debugHud_.Update(debugHudData_, sceMng_.GetDeltaTime());
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

	//debugHud_.Draw(debugHudData_);
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

bool GameScene::IsDebugKeyTriggered(int key)
{
	const bool isNow = CheckHitKey(key) != 0;
	bool& isPrev = debugKeyPrev_[key];
	const bool ret = isNow && !isPrev;
	isPrev = isNow;
	return ret;
}
