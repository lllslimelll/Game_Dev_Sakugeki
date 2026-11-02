#include "../Common/Transform.h"
#include "../GameObject/GameObject.h"
#include "../Renderer/Material.h"
#include "../Renderer/ModelRenderer.h"
#include "MeshRendererComponent.h"

MeshRendererComponent::MeshRendererComponent(GameObject& gameObject,
	ResourceManager::SRC vertexShader, ResourceManager::SRC pixelShader)
	:
	Component(gameObject),
	material_(std::make_unique<Material>(vertexShader, 0, pixelShader, 0)),
	renderer_(std::make_unique<ModelRenderer>(*material_, gameObject.GetTransform().modelId))
{
}

MeshRendererComponent::~MeshRendererComponent(void)
{
}

void MeshRendererComponent::Draw(void)
{
	renderer_->Draw();
}