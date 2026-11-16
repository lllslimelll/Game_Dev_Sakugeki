#include "../Common/Transform.h"
#include "../GameObject/GameObject.h"
#include "../../Engine/Component/Mesh.h"
#include "../Renderer/Material.h"
#include "../Renderer/ModelRenderer.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(GameObject& gameObject, const Mesh& mesh,
	ResourceManager::SRC vertexShader, ResourceManager::SRC pixelShader)
	:
	Component(gameObject),
	mesh_(mesh),
	material_(std::make_unique<Material>(vertexShader, 0, pixelShader, 0)),
	renderer_(std::make_unique<ModelRenderer>(*material_))
{
}

MeshRenderer::~MeshRenderer(void)
{
}

void MeshRenderer::Draw(void)
{
	renderer_->Draw(mesh_.GetHandle());
}