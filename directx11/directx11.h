#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl\client.h>
#include <DirectXMath.h>

#include "..\graphics.h"
#include "..\window.h"

#include "shader-manager.h"
#include "texture-manager.h"

#pragma comment(lib, "d3d11.lib")

using namespace Microsoft::WRL;

class __declspec(dllexport) DirectX11 : public Graphics
{
public:
	struct Model : public IModel
	{
		struct Mesh
		{
			ComPtr<ID3D11ShaderResourceView> srv_;
			ComPtr<ID3D11Buffer> vertex_buffer_;
			ComPtr<ID3D11Buffer> index_buffer_;
			unsigned int index_cnt_;

			Mesh(void)
				: index_cnt_(0U)
			{}
		};

		std::vector<Mesh> meshes_;
	};

public:
	DirectX11(Window * const window)
		: Graphics(window)
		, frame_rate_(FRAME_RATE_LIMITED)
	{
		this->set_shader_manager<Dx11ShaderManager>(this);
		this->set_texture_manager<Dx11TextureManager>(this);
	}

private:
	ComPtr<ID3D11Device> device_;
	ComPtr<ID3D11DeviceContext> context_;
	ComPtr<IDXGISwapChain> swap_chain_;

public:
	const ComPtr<ID3D11Device> & device(void) const;
	const ComPtr<ID3D11DeviceContext> & context(void) const;

private:
	FRAME_RATE frame_rate_;

public:
	void Initialize(void) override;
	void Finalize(void) override;
	void Clear(void) override;
	void Present(void) override;
	void Rendering(const std::weak_ptr<Renderer> & renderer) override;

private:
	ComPtr<ID3D11RenderTargetView> back_buffer_rtv_;

	ComPtr<ID3D11DepthStencilView> dsv_;

	D3D11_VIEWPORT viewport_;

	struct SimpleVertex
	{
		SimpleVertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT2 uv)
			: position_(position), normal_(normal), uv_(uv) {}
		DirectX::XMFLOAT3 position_;
		DirectX::XMFLOAT3 normal_;
		DirectX::XMFLOAT2 uv_;
	};

public:
	std::vector<SimpleVertex> vertices_;
	std::vector<unsigned int> indices_;
	Model field_;

private:
	ComPtr<ID3D11RasterizerState> rasterizer_states_[RASTERIZER_STATE_NUM];
	ComPtr<ID3D11BlendState> blend_states_[BLEND_STATE_NUM];
	ComPtr<ID3D11SamplerState> sampler_states_[SAMPLER_STATE_NUM];
	ComPtr<ID3D11Buffer> quad_vb_;

private:
	void CreateBackBuffer(void);
	void CreateDepthStencilView(void);
	void CreateViewPort(void);
	void CreateRasterizerStates(void);
	void CreateBlendStates(void);
	void CreateSamplerStates(void);
	void CreateQuad(void);

private:
	void CreateBlendState(D3D11_BLEND src_blend, D3D11_BLEND dest_blend, BLEND_STATE blend_state);
	void CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode, SAMPLER_STATE sampler_state);

public:
	void BackBuffer2D(void) override;
	void BackBuffer3D(void) override;
	void BackBufferDisplacement(void) override;
	void Deffered2D(void) override;
	void Deffered3D(void) override;
	void DefferedDisplacement(void) override;
	void ShadowMap(void) override;
	void ShadowMapDisplacement(void) override;

public:
	void LoadModel(const Resource::Model::PATH & path, std::shared_ptr<IModel> & model) override;

private:
	template<class _Vertex> void CreateVertexBuffer(DirectX11::Model::Mesh & mesh, std::vector<_Vertex> & vertices)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = vertices.size() * sizeof(_Vertex);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();

		this->device_->CreateBuffer(&bd, &sd, mesh.vertex_buffer_.GetAddressOf());
	}
	template<class _Index> void CreateIndexBuffer(DirectX11::Model::Mesh & mesh, std::vector<_Index> & indices)
	{
		mesh.index_cnt_ = indices.size();

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = mesh.index_cnt_ * sizeof(_Index);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = indices.data();

		this->device_->CreateBuffer(&bd, &sd, mesh.index_buffer_.GetAddressOf());
	}

public:
	void Destroy(void) override;
};

Graphics __declspec(dllexport) * const Create(const Window * window);