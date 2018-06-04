#include "directx11.h"
#include <iostream>

DirectX11::DirectX11(const Window * window)
	: window_(window)
	, frame_rate_(FRAME_RATE_LIMITED)
{
}

const ComPtr<ID3D11Device> & DirectX11::device(void)
{
	return this->device_;
}

void DirectX11::Initialize(void)
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = this->window_->width();
	sd.BufferDesc.Height = this->window_->height();
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = this->window_->hwnd();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL * feature_level = nullptr;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		0, &feature_levels, 1, D3D11_SDK_VERSION, &sd, &this->swap_chain_, &this->device_,
		feature_level, &this->context_);

	this->CreateBackBuffer();
	this->CreateDepthStencilView();
	this->CreateViewPort();
	this->CreateRasterizerStates();
	this->CreateBlendStates();
}

void DirectX11::Finalize(void)
{
}

void DirectX11::Clear(void)
{
	DirectX::XMVECTOR clear_color = { .2f, .4f, .8f, 1.f };
	this->context_->ClearRenderTargetView(this->back_buffer_rtv_.Get(), (float*)&clear_color);
	this->context_->ClearDepthStencilView(this->dsv_.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void DirectX11::Present(void)
{
	this->swap_chain_->Present(this->frame_rate_, 0);
}

void DirectX11::Rendering(const std::weak_ptr<Renderer>& renderer)
{
	//auto r = renderer.lock();
	//auto & rm = ResourceManager::Get();
	//auto & shader = rm.GetShader<Dx11Shader>(r->shader_);

	//this->context_->RSSetState(this->rasterizer_states_[r->rasterizer_state_].Get());

	//this->context_->OMSetBlendState(this->blend_states_[r->blend_state_].Get(), nullptr, 0xffffffff);

	//this->context_->PSSetSamplers(0, 1, this->sampler_states_[r->sampler_state_].GetAddressOf());


	//this->context_->VSSetShader(shader->vertex_shader_.Get(), nullptr, 0);
	//this->context_->GSSetShader(shader->geometry_shader_.Get(), nullptr, 0);
	//this->context_->PSSetShader(shader->pixel_shader_.Get(), nullptr, 0);

	//this->context_->IASetInputLayout(shader->input_layout_.Get());

	//unsigned int stride = 32U;
	//unsigned int offset = 0;

	//this->context_->UpdateSubresource(shader->constant_buffer_[0].Get(), 0, nullptr, r->constant_buffer_, 0, 0);

	////このコンスタントバッファーをどのシェーダーで使うか
	//this->context_->VSSetConstantBuffers(0, 1, shader->constant_buffer_[0].GetAddressOf());
	//this->context_->GSSetConstantBuffers(0, 1, shader->constant_buffer_[0].GetAddressOf());
	//this->context_->PSSetConstantBuffers(0, 1, shader->constant_buffer_[0].GetAddressOf());

	//if (r->draw_mode_ == DRAW_MODE_BACK_BUFFER_2D || r->draw_mode_ == DRAW_MODE_DEFFERED_2D)
	//{
	//	auto & texture = rm.GetTexture<Dx11Texture>(r->texture_2d_);

	//	this->context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//	this->context_->PSSetShaderResources(0, 1, texture->srv_.GetAddressOf());
	//	this->context_->IASetVertexBuffers(0, 1, texture->vertex_buffer_.GetAddressOf(), &stride, &offset);
	//	this->context_->Draw(4, 0);
	//}
	//else
	//{
	//	auto & model = rm.GetModel<Dx11Model>(r->model_);

	//	this->context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	for (auto & mesh : model->meshes_)
	//	{
	//		this->context_->IASetVertexBuffers(0, 1, mesh.vertex_buffer_.GetAddressOf(), &stride, &offset);
	//		this->context_->IASetIndexBuffer(mesh.index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
	//		this->context_->DrawIndexed(mesh.index_cnt_, 0, 0);
	//	}
	//}
}

void DirectX11::CreateBackBuffer(void)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex_2d;

	// バックバッファーテクスチャーを取得
	this->swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&tex_2d);

	// そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	this->device_->CreateRenderTargetView(tex_2d.Get(), nullptr, this->back_buffer_rtv_.GetAddressOf());
}

void DirectX11::CreateDepthStencilView(void)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex_2d;

	//深度マップテクスチャをレンダーターゲットにする際のデプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = this->window_->width();
	tex_desc.Height = this->window_->height();
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_D32_FLOAT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;

	this->device_->CreateTexture2D(&tex_desc, nullptr, tex_2d.GetAddressOf());
	this->device_->CreateDepthStencilView(tex_2d.Get(), nullptr, this->dsv_.GetAddressOf());
}

void DirectX11::CreateViewPort(void)
{
	auto & vp = this->viewport_;

	vp.Width = this->window_->width<float>();
	vp.Height = this->window_->height<float>();
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
}

void DirectX11::CreateRasterizerStates(void)
{
	{
		D3D11_RASTERIZER_DESC desc = {};

		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = true;

		HRESULT hr = this->device_->CreateRasterizerState(&desc, this->rasterizer_states_[RASTERIZER_STATE_CULL_NONE].GetAddressOf());

		if (FAILED(hr))
			std::cout << "Failed on create: RasterizerState - cull_none" << std::endl;
	}

	{
		D3D11_RASTERIZER_DESC desc = {};

		desc.CullMode = D3D11_CULL_FRONT;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = true;

		HRESULT hr = this->device_->CreateRasterizerState(&desc, this->rasterizer_states_[RASTERIZER_STATE_CW].GetAddressOf());

		if (FAILED(hr))
			std::cout << "Failed on create: RasterizerState - cull_clock_wise" << std::endl;
	}

	{
		D3D11_RASTERIZER_DESC desc = {};

		desc.CullMode = D3D11_CULL_BACK;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = true;

		HRESULT hr = this->device_->CreateRasterizerState(&desc, this->rasterizer_states_[RASTERIZER_STATE_CCW].GetAddressOf());

		if (FAILED(hr))
			std::cout << "Failed on create: RasterizerState - cull_counter_clock_wise" << std::endl;
	}

	{
		D3D11_RASTERIZER_DESC desc = {};

		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = true;

		HRESULT hr = this->device_->CreateRasterizerState(&desc, this->rasterizer_states_[RASTERIZER_STATE_WIREFRAME].GetAddressOf());

		if (FAILED(hr))
			std::cout << "Failed on create: RasterizerState - wire_frame" << std::endl;
	}
}

void DirectX11::CreateBlendStates(void)
{
	this->CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, BLEND_STATE_OPAQUE);
	this->CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, BLEND_STATE_ALPHA_BLEND);
	this->CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, BLEND_STATE_ADD);
	this->CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, BLEND_STATE_NON_PREMULTIPLIED);
}

void DirectX11::CreateSamplerStates(void)
{
	this->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, SAMPLER_STATE_POINT_WRAP);
	this->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, SAMPLER_STATE_POINT_CLAMP);
	this->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, SAMPLER_STATE_POINT_WRAP);
	this->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, SAMPLER_STATE_POINT_CLAMP);
	this->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, SAMPLER_STATE_POINT_WRAP);
	this->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, SAMPLER_STATE_POINT_CLAMP);
}

void DirectX11::CreateBlendState(D3D11_BLEND src_blend, D3D11_BLEND dest_blend, BLEND_STATE blend_state)
{
	D3D11_BLEND_DESC desc = {};

	desc.RenderTarget[0].BlendEnable = (src_blend != D3D11_BLEND_ONE) ||
		(dest_blend != D3D11_BLEND_ZERO);

	desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = src_blend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = dest_blend;
	desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	auto hr = this->device_->CreateBlendState(&desc, &this->blend_states_[blend_state]);

	if (FAILED(hr))
		std::cout << "Failed on create: BlendState - " << blend_state << std::endl;
}

void DirectX11::CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode, SAMPLER_STATE sampler_state)
{
	D3D11_SAMPLER_DESC desc = {};

	desc.Filter = filter;

	desc.AddressU = mode;
	desc.AddressV = mode;
	desc.AddressW = mode;

	desc.MaxAnisotropy = (this->device_->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? D3D11_MAX_MAXANISOTROPY : 2;

	desc.MaxLOD = FLT_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT hr = this->device_->CreateSamplerState(&desc, this->sampler_states_[sampler_state].GetAddressOf());

	if (FAILED(hr))
		std::cout << "Failed on create: SamplerState - " << sampler_state << std::endl;
}

void DirectX11::SetupBackBuffer(void)
{
	this->context_->OMSetRenderTargets(1, this->back_buffer_rtv_.GetAddressOf(), this->dsv_.Get());
	this->context_->RSSetViewports(1, &this->viewport_);
}

void DirectX11::SetupDeffered(void)
{
}

void DirectX11::SetupShadowMap(void)
{
}

void DirectX11::Setup2D(void)
{

}

void DirectX11::Setup3D(void)
{
}

void DirectX11::Destroy(void)
{
	delete this;
}

Graphics * Create(const Window * window)
{
	return new DirectX11(window);
}
