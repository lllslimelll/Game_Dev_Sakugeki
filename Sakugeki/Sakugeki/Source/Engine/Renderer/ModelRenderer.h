#pragma once
#include <DxLib.h>
#include "../Math/Vector2.h"
#include "Material.h"

class ModelRenderer
{
public:

	// コンストラクタ
	ModelRenderer(Material& material, int& modelH);

	// デストラクタ
	~ModelRenderer(void);

	// 描画
	void Draw(void);

private:

	// モデルマテリアル
	Material& material_; 

	// モデルのハンドル
	int& modelH_;

	// シェーダ設定（頂点）
	void SetReserveVS(void);
	// シェーダ設定（ピクセル）
	void SetReservePS(void);
};
