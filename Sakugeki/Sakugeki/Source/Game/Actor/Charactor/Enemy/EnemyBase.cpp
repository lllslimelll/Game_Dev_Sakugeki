#include "../../../../Engine/Utility/AsoUtility.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const EnemyBase::EnemyData& data, Player* player)
	:
	CharactorBase(),
	player_(player),
	type_(data.type),
	hp_(data.hp),
	stateBase_(-1),
	defaultPos_(data.defaultPos)
{
	// 初期座標の設定
	transform_.pos = data.defaultPos;
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Draw(void)
{
	CharactorBase::Draw();

#ifdef _DEBUG

	// 移動可能範囲のデバッグ範囲
	//DrawSphere3D(defaultPos_, movableRange_, 16, 0x000099, 0x000099, false);

#endif // _DEBUG
}

void EnemyBase::ChangeState(int state)
{
	stateBase_ = state;

	// 各状態遷移の初期処理
	stateChanges_[stateBase_]();
}