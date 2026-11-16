#include <DxLib.h>
#include "../Common/Transform.h"
#include "../GameObject/GameObject.h"
#include "Mesh.h"

Mesh::Mesh(GameObject& gameObject)
	:
	Component(gameObject),
	handle_(-1)
{
}

Mesh::~Mesh(void)
{
}

void Mesh::Init(void)
{
	ApplyTransform();
}

void Mesh::Update(void)
{
	ApplyTransform();
}

void Mesh::SetHandle(int handle)
{
	handle_ = handle;
}

int Mesh::GetHandle(void) const
{
	return handle_;
}

bool Mesh::HasHandle(void) const
{
	return handle_ != -1;
}

void Mesh::ApplyTransform(void)
{
	if (!HasHandle()) { return; }

	// ‚¿å‚Ì Transform ‚Ìs—ñ‚ğƒ‚ƒfƒ‹‚É”½‰f‚·‚é
	MV1SetMatrix(handle_, gameObject_.GetTransform().GetMatrix().ToMATRIX());
}