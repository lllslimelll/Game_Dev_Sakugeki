#include <cassert>
#include <DxLib.h>
#include "ResourcePath.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	Resource* res;

	// モデル
	//------------------------------------------------------------------------------------------------- 
	// プレイヤー
	res = new Resource(Resource::TYPE::MODEL, ResourcePath::PATH_MODELS + L"Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);
	// 地面
	res = new Resource(Resource::TYPE::MODEL, ResourcePath::PATH_MODELS + L"Stage/Ground.mv1");
	resourcesMap_.emplace(SRC::STAGE_GROUND, res);
	// 木
	res = new Resource(Resource::TYPE::MODEL, ResourcePath::PATH_MODELS + L"Stage/Tree.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE, res);
	//------------------------------------------------------------------------------------------------- 

	// 音源
	//------------------------------------------------------------------------------------------------- 
	// タイトルBGM
	res = new Resource(Resource::TYPE::MODEL, ResourcePath::PATH_AUDIO + L"BGM/Title.mv1");
	resourcesMap_.emplace(SRC::BGM_TITLE, res);
	//------------------------------------------------------------------------------------------------- 
	
	// シェーダー
	//------------------------------------------------------------------------------------------------- 
	// プレイヤー
	res = new Resource(Resource::TYPE::SHADER_VS, ResourcePath::PATH_SHADERS + L"PlayerVS.vso");
	resourcesMap_.emplace(SRC::SHADER_VS_PLAYER, res);
	res = new Resource(Resource::TYPE::SHADER_PS, ResourcePath::PATH_SHADERS + L"PlayerPS.pso");
	resourcesMap_.emplace(SRC::SHADER_PS_PLAYER, res);
	// 地面
	res = new Resource(Resource::TYPE::SHADER_VS, ResourcePath::PATH_SHADERS + L"GroundVS.vso");
	resourcesMap_.emplace(SRC::SHADER_VS_GROUND, res);
	res = new Resource(Resource::TYPE::SHADER_PS, ResourcePath::PATH_SHADERS + L"GroundPS.pso");
	resourcesMap_.emplace(SRC::SHADER_PS_GROUND, res);
	// 木
	res = new Resource(Resource::TYPE::SHADER_VS, ResourcePath::PATH_SHADERS + L"TreeVS.vso");
	resourcesMap_.emplace(SRC::SHADER_VS_TREE, res);
	res = new Resource(Resource::TYPE::SHADER_PS, ResourcePath::PATH_SHADERS + L"TreePS.pso");
	resourcesMap_.emplace(SRC::SHADER_PS_TREE, res);
	//------------------------------------------------------------------------------------------------- 

	// アニメーション
	//------------------------------------------------------------------------------------------------- 
	// 待機
	res = new Resource(Resource::TYPE::ANIMATION, ResourcePath::PATH_ANIMATIONS + L"Player/Idle.mv1");
	resourcesMap_.emplace(SRC::ANIM_IDLE, res);
	// 走る
	res = new Resource(Resource::TYPE::ANIMATION, ResourcePath::PATH_ANIMATIONS + L"Player/Run.mv1");
	resourcesMap_.emplace(SRC::ANIM_RUN, res);
	// ジャンプ
	res = new Resource(Resource::TYPE::ANIMATION, ResourcePath::PATH_ANIMATIONS + L"Player/Jump.mv1");
	resourcesMap_.emplace(SRC::ANIM_JUMP, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		delete res.second;
	}
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	const Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

int ResourceManager::GetHanlde(SRC src)
{
	auto it = loadedMap_.find(src);
	assert(it != loadedMap_.end() && "ハンドル取得前にロードが必要");
	if (it == loadedMap_.end())
	{
		return -1;
	}
	return it->second.handleId_;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{
	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
