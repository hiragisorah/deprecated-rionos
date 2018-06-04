#include "graphics.h"

void Graphics::AddRenderer(const std::shared_ptr<Renderer> & renderer)
{
	this->renderer_list_[renderer->draw_mode_].emplace_back(renderer);
}

bool Graphics::Run(void)
{
	this->Clear();

	this->SetupShadowMap();
	this->Setup3D();

	for (auto & renderer : this->renderer_list_[DRAW_MODE_SHADOW_MAP])
		this->Rendering(renderer);

	this->SetupDeffered();
	this->Setup3D();

	for (auto & renderer : this->renderer_list_[DRAW_MODE_DEFFERED_3D])
		this->Rendering(renderer);

	this->Setup2D();

	for (auto & renderer : this->renderer_list_[DRAW_MODE_DEFFERED_2D])
		this->Rendering(renderer);

	this->SetupBackBuffer();
	this->Setup3D();

	for (auto & renderer : this->renderer_list_[DRAW_MODE_BACK_BUFFER_3D])
		this->Rendering(renderer);

	this->Setup2D();

	for (auto & renderer : this->renderer_list_[DRAW_MODE_BACK_BUFFER_2D])
		this->Rendering(renderer);

	this->Present();

	return true;
}
