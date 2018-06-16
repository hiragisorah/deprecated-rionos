#pragma once

#include <wrl\client.h>
#include <d3d11.h>

#include <vector>

#include "..\shader-manager.h"

using namespace Microsoft::WRL;

class DirectX11;

struct Shader : public IShader
{
	ComPtr<ID3D11VertexShader> vertex_shader_;
	ComPtr<ID3D11GeometryShader> geometry_shader_;
	ComPtr<ID3D11HullShader> hull_shader_;
	ComPtr<ID3D11DomainShader> domain_shader_;
	ComPtr<ID3D11PixelShader> pixel_shader_;
	ComPtr<ID3D11InputLayout> input_layout_;
	std::vector<ComPtr<ID3D11Buffer>> constant_buffer_;
};

class Dx11ShaderManager : public ShaderManager
{
public:
	Dx11ShaderManager(DirectX11 * const graphics);

private:
	void LoadShader(const Resource::Shader::PATH & path, std::shared_ptr<IShader> & shader) override;

private:
	DirectX11 * const graphics_;

private:
	void CreateInputLayoutAndConstantBufferFromShader(const std::shared_ptr<Shader> & shader, ID3DBlob * blob);
	DXGI_FORMAT GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask);
};