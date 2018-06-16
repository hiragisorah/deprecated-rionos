#include "directx11.h"

#include <fstream>
#include <iostream>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "directxtex.lib")

const ComPtr<ID3D11Device> & DirectX11::device(void) const
{
	return this->device_;
}

const ComPtr<ID3D11DeviceContext> & DirectX11::context(void) const
{
	return this->context_;
}

void DirectX11::Initialize(void)
{
	// デバイスとスワップチェーンの作成 
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = this->window()->width();
	sd.BufferDesc.Height = this->window()->height();
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = this->window()->hwnd();
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
	this->CreateQuad();
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
	auto r = renderer.lock();
	auto shader = this->shader_manager()->Get<Shader>(r->shader_);

	this->context_->RSSetState(this->rasterizer_states_[r->rasterizer_state_].Get());

	this->context_->OMSetBlendState(this->blend_states_[r->blend_state_].Get(), nullptr, 0xffffffff);

	this->context_->PSSetSamplers(0, 1, this->sampler_states_[r->sampler_state_].GetAddressOf());

	this->context_->VSSetShader(shader->vertex_shader_.Get(), nullptr, 0);
	this->context_->GSSetShader(shader->geometry_shader_.Get(), nullptr, 0);
	this->context_->HSSetShader(shader->hull_shader_.Get(), nullptr, 0);
	this->context_->DSSetShader(shader->domain_shader_.Get(), nullptr, 0);
	this->context_->PSSetShader(shader->pixel_shader_.Get(), nullptr, 0);

	if (shader->constant_buffer_.size())
	{
		this->context_->VSSetConstantBuffers(0, shader->constant_buffer_.size(), shader->constant_buffer_[0].GetAddressOf());
		this->context_->GSSetConstantBuffers(0, shader->constant_buffer_.size(), shader->constant_buffer_[0].GetAddressOf());
		this->context_->HSSetConstantBuffers(0, shader->constant_buffer_.size(), shader->constant_buffer_[0].GetAddressOf());
		this->context_->DSSetConstantBuffers(0, shader->constant_buffer_.size(), shader->constant_buffer_[0].GetAddressOf());
		this->context_->PSSetConstantBuffers(0, shader->constant_buffer_.size(), shader->constant_buffer_[0].GetAddressOf());
		this->context_->UpdateSubresource(shader->constant_buffer_[0].Get(), 0, nullptr, r->constant_buffer_, 0, 0);
	}

	this->context_->IASetInputLayout(shader->input_layout_.Get());

	if (r->draw_mode_ == DRAW_MODE_BACK_BUFFER_2D || r->draw_mode_ == DRAW_MODE_DEFFERED_2D)
	{
		unsigned int stride = 20U;
		unsigned int offset = 0;

		auto & texture = this->texture_manager_->Get<Texture>(r->texture_2d_[0]);

		this->context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		for (unsigned int n = 0; n < r->texture_2d_.size(); ++n)
		{
			auto tex = this->texture_manager_->Get<Texture>(r->texture_2d_[n]);

			this->context_->HSSetShaderResources(n, 1, tex->srv_.GetAddressOf());
			this->context_->DSSetShaderResources(n, 1, tex->srv_.GetAddressOf());
			this->context_->PSSetShaderResources(n, 1, tex->srv_.GetAddressOf());
		}

		this->context_->IASetVertexBuffers(0, 1, texture->vertex_buffer_.GetAddressOf(), &stride, &offset);
		this->context_->Draw(4, 0);
	}
	else if (r->draw_mode_ == DRAW_MODE_BACK_BUFFER_DISP || r->draw_mode_ == DRAW_MODE_DEFFERED_DISP || r->draw_mode_ == DRAW_MODE_SHADOW_MAP_DISP)
	{
		unsigned int stride = 20U;
		unsigned int offset = 0;

		this->context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

		for (unsigned int n = 0; n < r->texture_2d_.size(); ++n)
		{
			auto tex = this->texture_manager_->Get<Texture>(r->texture_2d_[n]);

			this->context_->HSSetShaderResources(n, 1, tex->srv_.GetAddressOf());
			this->context_->DSSetShaderResources(n, 1, tex->srv_.GetAddressOf());
			this->context_->PSSetShaderResources(n, 1, tex->srv_.GetAddressOf());
		}

		this->context_->IASetVertexBuffers(0, 1, this->quad_vb_.GetAddressOf(), &stride, &offset);
		this->context_->Draw(4, 0);
	}
	else
	{
		unsigned int stride = 32U;
		unsigned int offset = 0;

		auto & model = this->GetModel<DirectX11::Model>(r->model_);

		this->context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (auto & mesh : model->meshes_)
		{
			this->context_->IASetVertexBuffers(0, 1, mesh.vertex_buffer_.GetAddressOf(), &stride, &offset);
			this->context_->IASetIndexBuffer(mesh.index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
			this->context_->DrawIndexed(mesh.index_cnt_, 0, 0);
		}
	}
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
	tex_desc.Width = this->window()->width();
	tex_desc.Height = this->window()->height();
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

	vp.Width = this->window()->width<float>();
	vp.Height = this->window()->height<float>();
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
	this->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, SAMPLER_STATE_LINEAR_WRAP);
	this->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, SAMPLER_STATE_LINEAR_CLAMP);
	this->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, SAMPLER_STATE_ANISOTROPIC_WRAP);
	this->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, SAMPLER_STATE_ANISOTROPIC_CLAMP);
}

void DirectX11::CreateQuad(void)
{
	struct Vtx
	{
		DirectX::XMFLOAT3 position_;
		DirectX::XMFLOAT2 uv_;
	};

	//頂点を定義
	Vtx vertices[] =
	{
		DirectX::XMFLOAT3(-1, 0, +1),DirectX::XMFLOAT2(0,0),
		DirectX::XMFLOAT3(+1, 0, +1),DirectX::XMFLOAT2(0,1),
		DirectX::XMFLOAT3(+1, 0, -1),DirectX::XMFLOAT2(1,1),
		DirectX::XMFLOAT3(-1, 0, -1),DirectX::XMFLOAT2(1,0),
	};

	//上の頂点でバーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vtx) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	this->device_->CreateBuffer(&bd, &InitData, &this->quad_vb_);
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

	desc.MaxAnisotropy = 16;
	desc.MinLOD = 0;
	desc.MaxLOD = FLT_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT hr = this->device_->CreateSamplerState(&desc, this->sampler_states_[sampler_state].GetAddressOf());

	if (FAILED(hr))
		std::cout << "Failed on create: SamplerState - " << sampler_state << std::endl;
}

void DirectX11::BackBuffer2D(void)
{
	this->context_->OMSetRenderTargets(1, this->back_buffer_rtv_.GetAddressOf(), this->dsv_.Get());
	this->context_->RSSetViewports(1, &this->viewport_);
}

void DirectX11::BackBuffer3D(void)
{
	this->context_->OMSetRenderTargets(1, this->back_buffer_rtv_.GetAddressOf(), this->dsv_.Get());
	this->context_->RSSetViewports(1, &this->viewport_);
}

void DirectX11::BackBufferDisplacement(void)
{
	this->context_->OMSetRenderTargets(1, this->back_buffer_rtv_.GetAddressOf(), this->dsv_.Get());
	this->context_->RSSetViewports(1, &this->viewport_);
}

void DirectX11::Deffered2D(void)
{
}

void DirectX11::Deffered3D(void)
{
}

void DirectX11::DefferedDisplacement(void)
{
	this->context_->OMSetRenderTargets(1, this->back_buffer_rtv_.GetAddressOf(), this->dsv_.Get());
	this->context_->RSSetViewports(1, &this->viewport_);
}

void DirectX11::ShadowMap(void)
{
}

void DirectX11::ShadowMapDisplacement(void)
{
	this->context_->OMSetRenderTargets(1, this->back_buffer_rtv_.GetAddressOf(), this->dsv_.Get());
	this->context_->RSSetViewports(1, &this->viewport_);
}

using namespace DirectX;

struct SkinMesh
{
	XMFLOAT3 position_;
	XMFLOAT3 color_;
	XMFLOAT3 normal_;
	unsigned int indices_[4];
	XMFLOAT3 weights_;
};

struct Bone
{
	DirectX::XMMATRIX init_;
};

void DirectX11::LoadModel(const Resource::Model::PATH & path, std::shared_ptr<IModel>& model)
{
	auto dx_model = std::make_shared<DirectX11::Model>();
	model = dx_model;

	std::string file_path = Resource::Model::paths[(unsigned int)path];

	struct Vertex
	{
		DirectX::XMFLOAT3 position_;
		DirectX::XMFLOAT3 normal_;
		DirectX::XMFLOAT2 uv_;
	};

	struct Mesh
	{
		std::string texture_;
		std::vector<Vertex> vertices_;
		std::vector<unsigned int> indices_;
	};

	std::vector<Mesh> meshes;

	meshes.clear();

	std::ifstream file;

	file.open(file_path, std::ios::in | std::ios::binary);

	if (file.fail())
	{
		std::cout << "モデルファイルの読み込みに失敗しました。" << std::endl;
		return;
	}

	unsigned int mesh_cnt;

	file.read(reinterpret_cast<char*>(&mesh_cnt), sizeof(unsigned int));

	meshes.resize(mesh_cnt);
	dx_model->meshes_.resize(mesh_cnt);

	for (unsigned int m = 0; m < mesh_cnt; ++m)
	{
		auto & mesh = meshes[m];

		unsigned int vtx_cnt = 0;

		file.read(reinterpret_cast<char*>(&vtx_cnt), sizeof(unsigned int));

		mesh.vertices_.resize(vtx_cnt);

		for (unsigned int v = 0; v < vtx_cnt; ++v)
		{
			auto & vtx = mesh.vertices_[v];

			file.read(reinterpret_cast<char*>(&vtx), sizeof(Vertex));
		}

		unsigned int index_cnt = 0;

		file.read(reinterpret_cast<char*>(&index_cnt), sizeof(unsigned int));

		mesh.indices_.resize(index_cnt);

		for (unsigned int i = 0; i < index_cnt; ++i)
		{
			auto & index = mesh.indices_[i];

			file.read(reinterpret_cast<char*>(&index), sizeof(unsigned int));
		}

		unsigned texture_str_cnt = 0;

		file.read(reinterpret_cast<char*>(&texture_str_cnt), sizeof(unsigned int));

		mesh.texture_.resize(texture_str_cnt);

		if (texture_str_cnt > 0)
		{
			char * texture_str = new char[texture_str_cnt + 1];
			file.read(&texture_str[0], sizeof(char) * texture_str_cnt);
			texture_str[texture_str_cnt] = '\0';
			mesh.texture_ = texture_str;
			if (mesh.texture_.find(".") < mesh.texture_.size())
				mesh.texture_ = mesh.texture_.substr(mesh.texture_.rfind("\\") + 1, mesh.texture_.size() - mesh.texture_.rfind("\\"));
			else
				mesh.texture_ = "";

			delete[] texture_str;
		}
	}

	file.close();

	for (unsigned int n = 0; n < mesh_cnt; ++n)
	{
		//if (meshes[n].texture_ != "")
		//{
		//	auto dir = file_path;
		//	dir.replace(dir.rfind("/"), dir.size() - dir.rfind("/"), "/" + meshes[n].texture_);
		//	std::wstring w_file_path = std::wstring(dir.begin(), dir.end());
		//	DirectX::CreateWICTextureFromFile(this->graphics_->device().Get(), w_file_path.c_str(), nullptr, dx_model->meshes_[n].srv_.GetAddressOf());
		//}

		this->CreateVertexBuffer(dx_model->meshes_[n], meshes[n].vertices_);
		this->CreateIndexBuffer(dx_model->meshes_[n], meshes[n].indices_);
	}
}

void DirectX11::Destroy(void)
{
	delete this;
}

Graphics * const Create(Window * const window)
{
	return new DirectX11(window);
}
