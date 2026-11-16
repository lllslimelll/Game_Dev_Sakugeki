#pragma once
#include <DxLib.h>
#include "../Math/Vector2.h"
#include "Material.h"

class ModelRenderer
{
public:

	// コンストラクタ
	ModelRenderer(Material& material);

	// デストラクタ
	~ModelRenderer(void);

	// 描画
	void Draw(int modelHandle);

private:

	// モデルマテリアル
	Material& material_; 

	// シェーダ設定（頂点）
	void SetReserveVS(void);
	// シェーダ設定（ピクセル）
	void SetReservePS(void);
};
