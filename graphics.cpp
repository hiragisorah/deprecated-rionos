#include "graphics.h"

Window * const Graphics::window(void) const
{
	return this->window_;
}

bool Graphics::Run(void)
{
	this->Clear();

	this->ShadowMap();

	auto & draw_list = this->renderer_list_[DRAW_MODE_SHADOW_MAP];

	for (unsigned int n = 0; n < draw_list.size(); ++n)
	{
		auto & renderer = draw_list[n];

		if (renderer.expired())
			draw_list.erase(draw_list.begin() + n);
		else
			this->Rendering(renderer);
	}

	this->ShadowMapDisplacement();

	draw_list = this->renderer_list_[DRAW_MODE_SHADOW_MAP_DISP];

	for (unsigned int n = 0; n < draw_list.size(); ++n)
	{
		auto & renderer = draw_list[n];

		if (renderer.expired())
			draw_list.erase(draw_list.begin() + n);
		else
			this->Rendering(renderer);
	}

	this->Deffered3D();

	draw_list = this->renderer_list_[DRAW_MODE_DEFFERED_3D];

	for (unsigned int n = 0; n < draw_list.size(); ++n)
	{
		auto & renderer = draw_list[n];

		if (renderer.expired())
			draw_list.erase(draw_list.begin() + n);
		else
			this->Rendering(renderer);
	}

	this->DefferedDisplacement();

	draw_list = this->renderer_list_[DRAW_MODE_DEFFERED_DISP];

	for (unsigned int n = 0; n < draw_list.size(); ++n)
	{
		auto & renderer = draw_list[n];

		if (renderer.expired())
			draw_list.erase(draw_list.begin() + n);
		else
			this->Rendering(renderer);
	}

	this->Deffered2D();

	draw_list = this->renderer_list_[DRAW_MODE_DEFFERED_2D];

	for (unsigned int n = 0; n < draw_list.size(); ++n)
	{
		auto & renderer = draw_list[n];

		if (renderer.expired())
			draw_list.erase(draw_list.begin() + n);
		else
			this->Rendering(renderer);
	}

	this->BackBuffer3D();

	draw_list = this->renderer_list_[DRAW_MODE_BACK_BUFFER_3D];

	for (unsigned int n = 0; n < draw_list.size(); ++n)
	{
		auto & renderer = draw_list[n];

		if (renderer.expired())
			draw_list.erase(draw_list.begin() + n);
		else
			this->Rendering(renderer);
	}

	this->BackBufferDisplacement();

	draw_list = this->renderer_list_[DRAW_MODE_BACK_BUFFER_DISP];

	for (unsigned int n = 0; n < draw_list.size(); ++n)
	{
		auto & renderer = draw_list[n];

		if (renderer.expired())
			draw_list.erase(draw_list.begin() + n);
		else
			this->Rendering(renderer);
	}

	this->BackBuffer2D();

	draw_list = this->renderer_list_[DRAW_MODE_BACK_BUFFER_2D];

	for (unsigned int n = 0; n < draw_list.size(); ++n)
	{
		auto & renderer = draw_list[n];

		if (renderer.expired())
			draw_list.erase(draw_list.begin() + n);
		else
			this->Rendering(renderer);
	}

	this->Present();

	return true;
}
