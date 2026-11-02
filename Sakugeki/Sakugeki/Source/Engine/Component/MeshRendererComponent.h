#pragma once
#include <memory>
#include "../Resource/ResourceManager.h"
#include "Component.h"

class Material;
class ModelRenderer;

// モデルを描画する部品
// 持ち主の Transform のモデルを、指定したシェーダーで描画する
class MeshRendererComponent : public Component
{
public:

	// コンストラクタ
	// gameObject   : 持ち主の GameObject
	// vertexShader : 頂点シェーダー
	// pixelShader  : ピクセルシェーダー
	MeshRendererComponent(GameObject& gameObject,
		ResourceManager::SRC vertexShader, ResourceManager::SRC pixelShader);

	// デストラクタ
	~MeshRendererComponent(void) override;

	// 描画
	void Draw(void) override;

private:

	// マテリアル（renderer_ が参照するので、先に宣言する）
	std::unique_ptr<Material> material_;

	// 描画
	std::unique_ptr<ModelRenderer> renderer_;
};