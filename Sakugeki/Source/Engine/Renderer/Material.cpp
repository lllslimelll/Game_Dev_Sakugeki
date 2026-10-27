#include "../Resource/ResourceManager.h"
#include "Material.h"

Material::Material(ResourceManager::SRC vertexShader, int constBufFloat4SizeVS,
					ResourceManager::SRC pixelShader, int constBufFloat4SizePS)
	:
	constBufFloat4SizeVS_(constBufFloat4SizeVS),
	constBufFloat4SizePS_(constBufFloat4SizePS),
	texAddress_(TEXADDRESS::CLAMP)
{
	auto& resMng = ResourceManager::GetInstance();
	// シェーダハンドル取得
	shaderVSH_ = resMng.GetHanlde(vertexShader);
	shaderPSH_ = resMng.GetHanlde(pixelShader);

	// シェーダー用の定数バッファを作成
	constBufVSH_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS_); // 頂点
	constBufPSH_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS_); // ピクセル
}

Material::Material(ResourceManager::SRC pixelShader, int constBufFloat4SizePS)
	:
	constBufFloat4SizeVS_(),
	constBufFloat4SizePS_(constBufFloat4SizePS),
	texAddress_(TEXADDRESS::CLAMP)
{
	// シェーダハンドル取得
	shaderPSH_ = ResourceManager::GetInstance().GetHanlde(pixelShader);

	// ピクセルシェーダー用の定数バッファを作成
	constBufPSH_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS_);
}

Material::~Material(void)
{
	// 定数バッファの解放
	DeleteShaderConstantBuffer(constBufVSH_);
	DeleteShaderConstantBuffer(constBufPSH_);
}

void Material::AddConstBufVS(const FLOAT4& constBuf)
{
	if (constBufFloat4SizeVS_ > constBufsVS_.size())
	{
		constBufsVS_.emplace_back(constBuf);
	}
}

void Material::AddConstBufPS(const FLOAT4& constBuf)
{
	if (constBufFloat4SizePS_ > constBufsPS_.size())
	{
		constBufsPS_.emplace_back(constBuf);
	}
}

void Material::SetConstBufVS(int idx, const FLOAT4& constBuf)
{
	if (idx >= constBufsVS_.size())
	{
		return;
	}

	constBufsVS_[idx] = constBuf;
}

void Material::SetConstBufPS(int idx, const FLOAT4& constBuf)
{
	if (idx >= constBufsPS_.size())
	{
		return;
	}

	constBufsPS_[idx] = constBuf;
}

void Material::AddTextureBuf(int texDiffuse)
{
	// 現在のサイズをスロット番号として使う
	// 0番はモデルのデフォルトテクスチャ用なので1番から
	int slot = textures_.size() + 1;
	textures_[slot] = texDiffuse;
}

void Material::SetTextureBuf(int idx, int texDiffuse)
{

	if (idx >= textures_.size())
	{
		return;
	}

	textures_[idx] = texDiffuse;

}

int Material::GetShaderVSH(void) const
{
	return shaderVSH_;
}

int Material::GetShaderPSH(void) const
{
	return shaderPSH_;
}

int Material::GetConstBufVSH(void) const
{
	return constBufVSH_;
}

int Material::GetConstBufPSH(void) const
{
	return constBufPSH_;
}

const std::vector<FLOAT4>& Material::GetConstBufsVS(void) const
{
	return constBufsVS_;
}

const std::vector<FLOAT4>& Material::GetConstBufsPS(void) const
{
	return constBufsPS_;
}

const std::map<int, int>& Material::GetTextures(void) const
{
	return textures_;
}

Material::TEXADDRESS Material::GetTextureAddress(void) const
{
	return texAddress_;
}

void Material::SetTextureAddress(TEXADDRESS texA)
{
	texAddress_ = texA;
}
