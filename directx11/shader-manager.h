#pragma once

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

class ShaderManager
{

};