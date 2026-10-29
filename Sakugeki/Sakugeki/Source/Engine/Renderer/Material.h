#pragma once
#include <string>
#include <vector>
#include <map>
#include <DxLib.h>
#include "../Resource/ResourceManager.h"

class Material
{

public:

	// テクスチャアドレスタイプ
	enum class TEXADDRESS
	{
		NONE,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	// シェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7; // 頂点用
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4; // ピクセル用

	// 設定できる最大のテクスチャの数
	static constexpr int MAX_TEXTURES = 4;

	/// <summary>
	/// コンストラクタ（3Dモデル用）
	/// </summary>
	/// <param name="vs">使用する頂点シェーダ</param>
	/// <param name="constBufFloat4Size">頂点シェーダ用定数バッファの確保サイズ(FLOAT4をいくつ作るか)</param>
	/// <param name="vs">使用するピクセルシェーダ</param>
	/// <param name="constBufFloat4Size">ピクセルシェーダ用定数バッファの確保サイズ(FLOAT4をいくつ作るか)</param>
	Material(ResourceManager::SRC vertexShader, int constBufSizeVS,
			 ResourceManager::SRC pixelShader, int constBufSizePS);

	/// <summary>
	/// コンストラクタ（ポストエフェクト用）
	/// </summary>
	/// <param name="vs">使用するピクセルシェーダ</param>
	/// <param name="constBufFloat4SizePS">ピクセルシェーダ用定数バッファの確保サイズ</param>
	Material(ResourceManager::SRC pixelShader, int constBufFloat4SizePS);

	// デストラクタ
	~Material(void);

	// 定数バッファを追加
	void AddConstBufVS(const FLOAT4& constBuf); // 頂点
	void AddConstBufPS(const FLOAT4& constBuf); // ピクセル

	// 定数バッファを更新
	void SetConstBufVS(int idx, const FLOAT4& constBuf); // 頂点
	void SetConstBufPS(int idx, const FLOAT4& constBuf); // ピクセル

	// テクスチャ追加
	void AddTextureBuf(int texDiffuse);
	// テクスチャ更新
	void SetTextureBuf(int idx, int texDiffuse);

	// シェーダハンドル取得
	int GetShaderVSH(void) const; // 頂点
	int GetShaderPSH(void) const; // ピクセル

	// 定数バッファハンドル取得
	int GetConstBufVSH(void) const; // 頂点
	int GetConstBufPSH(void) const; // ピクセル

	// 定数バッファ
	const std::vector<FLOAT4>& GetConstBufsVS(void) const; // 頂点
	const std::vector<FLOAT4>& GetConstBufsPS(void) const; // ピクセル

	// テクスチャ取得
	const std::map<int, int>& GetTextures(void) const;

	// テクスチャアドレスを取得
	TEXADDRESS GetTextureAddress(void) const;

	// テクスチャアドレスを設定
	void SetTextureAddress(TEXADDRESS texA);

private:

	// シェーダハンドル
	int shaderVSH_; // 頂点
	int shaderPSH_; // ピクセル

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4SizeVS_; // 頂点用
	int constBufFloat4SizePS_; // ピクセル用

	// 定数バッファハンドル
	int constBufVSH_; // 頂点
	int constBufPSH_; // ピクセル

	// テクスチャアドレス
	TEXADDRESS texAddress_;

	// 定数バッファ
	std::vector<FLOAT4> constBufsVS_; // 頂点
	std::vector<FLOAT4> constBufsPS_; // ピクセル

	// 画像
	std::map<int, int> textures_;

};
