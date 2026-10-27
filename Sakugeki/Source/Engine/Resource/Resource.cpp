#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Resource.h"

Resource::Resource(void)
	:
	type_(TYPE::NONE),
	path_(L""),
	numX_(-1),
	numY_(-1),
	sizeX_(-1),
	sizeY_(-1),
	handleId_(-1),
	handleIds_(nullptr)
{
}

Resource::Resource(TYPE type, const std::wstring& path)
	:
	type_(type),
	path_(path),
	numX_(-1),
	numY_(-1),
	sizeX_(-1),
	sizeY_(-1),
	handleId_(-1),
	handleIds_(nullptr)
{
}

Resource::Resource(TYPE type, const std::wstring& path, int numX, int numY, int sizeX, int sizeY)
	:
	type_(type),
	path_(path),
	numX_(numX),
	numY_(numY),
	sizeX_(sizeX),
	sizeY_(sizeY),
	handleId_(-1),
	handleIds_(nullptr)
{
}

Resource::~Resource(void)
{
}

void Resource::Load(void)
{
	switch (type_)
	{
	case Resource::TYPE::ANIMATION:
		// アニメーション
		handleId_ = MV1LoadModel(path_.c_str());
		break;

	case Resource::TYPE::AUDIO:
		// 音源
		handleId_ = LoadSoundMem(path_.c_str());
		break;

	case Resource::TYPE::EFFEKSEER:
		// エフェクシア
		handleId_ = LoadEffekseerEffect(path_.c_str());
		break;

	case Resource::TYPE::FONT:
		// フォント
		handleId_ = AddFontResourceEx(path_.c_str(), FR_PRIVATE, NULL);
		break;

	case Resource::TYPE::MODEL:
		// モデル
		handleId_ = MV1LoadModel(path_.c_str());
		break;

	case Resource::TYPE::SHADER_PS:
		// 頂点シェーダー
		handleId_ = LoadPixelShader(path_.c_str()); 
		break;

	case Resource::TYPE::SHADER_VS:
		// 頂点シェーダー
		handleId_ = LoadVertexShader(path_.c_str());
		break;

	case Resource::TYPE::TEXTURE:
		// 画像
		handleId_ = LoadGraph(path_.c_str());
		break;

	case Resource::TYPE::TEXTURES:
		// 複数画像
		handleIds_ = new int[numX_ * numY_];
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;
	}
}

void Resource::Release(void)
{
	switch (type_)
	{
	case Resource::TYPE::ANIMATION:
		// アニメーション
		handleId_ = MV1DeleteModel(handleId_);
		break;

	case Resource::TYPE::AUDIO:
		// 音源
		DeleteSoundMem(handleId_);
		break;

	case Resource::TYPE::EFFEKSEER:
		// エフェクシア
		DeleteEffekseerEffect(handleId_);
		break;

	case Resource::TYPE::FONT:
		// フォント
		RemoveFontResourceEx(path_.c_str(), FR_PRIVATE, NULL);
		break;

	case Resource::TYPE::MODEL:
	{
		// モデル
		MV1DeleteModel(handleId_);
		for (auto id : duplicateModelIds_)
		{
			MV1DeleteModel(id);
		}
	}
		break;

	case TYPE::SHADER_VS:
	case TYPE::SHADER_PS:
		// シェーダー
		DeleteShader(handleId_); 
		break;

	case Resource::TYPE::TEXTURE:
		// 画像
		DeleteGraph(handleId_);
		break;

	case Resource::TYPE::TEXTURES:
	{
		// 複数画像
		int num = numX_ * numY_;
		for (int i = 0; i < num; i++)
		{
			DeleteGraph(handleIds_[i]);
		}
		delete[] handleIds_;
	}
		break;
	}
}

void Resource::CopyHandle(int* imgs) const
{
	if (handleIds_ == nullptr)
	{
		return;
	}

	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		imgs[i] = handleIds_[i];
	}
}
