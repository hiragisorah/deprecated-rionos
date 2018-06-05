#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl\client.h>
#include <DirectXMath.h>

#include "..\graphics.h"
#include "..\window.h"

#pragma comment(lib, "d3d11.lib")

using namespace Microsoft::WRL;

class __declspec(dllexport) DirectX11 : public Graphics
{
public:
	DirectX11(Window * const window)
		: Graphics(window)
		, frame_rate_(FRAME_RATE_LIMITED)
	{}

private:
	ComPtr<ID3D11Device> device_;
	ComPtr<ID3D11DeviceContext> context_;
	ComPtr<IDXGISwapChain> swap_chain_;

public:
	const ComPtr<ID3D11Device> & device(void);

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

private:
	ComPtr<ID3D11RasterizerState> rasterizer_states_[RASTERIZER_STATE_NUM];
	ComPtr<ID3D11BlendState> blend_states_[BLEND_STATE_NUM];
	ComPtr<ID3D11SamplerState> sampler_states_[SAMPLER_STATE_NUM];

private:
	void CreateBackBuffer(void);
	void CreateDepthStencilView(void);
	void CreateViewPort(void);
	void CreateRasterizerStates(void);
	void CreateBlendStates(void);
	void CreateSamplerStates(void);

private:
	void CreateBlendState(D3D11_BLEND src_blend, D3D11_BLEND dest_blend, BLEND_STATE blend_state);
	void CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode, SAMPLER_STATE sampler_state);

public:
	void BackBuffer2D(void) override;
	void BackBuffer3D(void) override;
	void Deffered2D(void) override;
	void Deffered3D(void) override;
	void ShadowMap(void) override;

private:
	ComPtr<ID3D11Buffer> quad_vb_;
	void CreateQuad(void);
	void DrawQuad(void);

public:
	void Destroy(void) override;
};

Graphics __declspec(dllexport) * const Create(const Window * window);