#include "../../../Engine/Utility/AsoUtility.h"
#include "MeshCollider.h"

MeshCollider::MeshCollider(GameObject& gameObject, CollisionManager& collisionMng, int layer)
	:
	Collider(gameObject, collisionMng, SHAPE::MESH, layer)
{
}

MeshCollider::~MeshCollider(void)
{
}

void MeshCollider::Init(void)
{
	// CollisionManager への登録
	Collider::Init();

	// モデルの衝突情報を構築する
	MV1SetupCollInfo(GetTransform().modelId, -1);
}

void MeshCollider::AddExcludeFrameIds(const std::wstring& name)
{
	// フレーム数を取得
	int num = MV1GetFrameNum(GetTransform().modelId);
	for (int i = 0; i < num; i++)
	{
		// フレーム名称を取得
		std::wstring frameName = MV1GetFrameName(GetTransform().modelId, i);

		if (frameName.find(name) != std::wstring::npos)
		{
			// 除外フレームに追加
			excludeFrameIds_.emplace_back(i);
		}
	}
}

void MeshCollider::ClearExcludeFrame(void)
{
	excludeFrameIds_.clear();
}

bool MeshCollider::IsExcludeFrame(int frameIdx) const
{
	// 除外判定
	if (std::find(
		excludeFrameIds_.begin(),
		excludeFrameIds_.end(),
		frameIdx) != excludeFrameIds_.end())
	{
		// 除外に該当する
		return true;
	}

	return false;
}


