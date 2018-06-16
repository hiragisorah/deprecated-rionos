#include "texture-manager.h"

#include "WICTextureLoader.h"
#include "directx11.h"

Dx11TextureManager::Dx11TextureManager(DirectX11 * const graphics)
	: graphics_(graphics)
{
}

void Dx11TextureManager::LoadTexture(const Resource::Texture::PATH & path, std::shared_ptr<ITexture>& texture)
{
	auto dx_texture = std::make_shared<Texture>();
	texture = dx_texture;

	CreateTexture(dx_texture, path);
	CreateVertexBufferFromTextureSize(dx_texture);
}

void Dx11TextureManager::CreateTexture(const std::shared_ptr<Texture>& texture, const Resource::Texture::PATH & path)
{
	ComPtr<ID3D11Resource> res;

	std::string p = Resource::Texture::paths[(unsigned int)path];

	std::wstring w_file_path = std::wstring(p.begin(), p.end());
	DirectX::CreateWICTextureFromFile(this->graphics_->device().Get(), w_file_path.c_str(), res.GetAddressOf(), texture->srv_.GetAddressOf());

	ComPtr<ID3D11Texture2D> tex;
	res.As(&tex);

	D3D11_TEXTURE2D_DESC desc;

	tex->GetDesc(&desc);

	{
		ComPtr<ID3D11Texture2D> new_tex;
		D3D11_TEXTURE2D_DESC new_desc = {};
		tex->GetDesc(&new_desc);
		new_desc.BindFlags = 0;
		new_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		new_desc.Usage = D3D11_USAGE_STAGING;
		new_desc.MiscFlags = 0;
		this->graphics_->device()->CreateTexture2D(&new_desc, nullptr, new_tex.GetAddressOf());
		this->graphics_->context()->CopyResource(new_tex.Get(), tex.Get());
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		unsigned int subres = D3D11CalcSubresource(0, 0, 0);
		this->graphics_->context()->Map(new_tex.Get(), subres, D3D11_MAP_READ, 0, &mapped);
		const std::uint8_t * sptr = reinterpret_cast<const uint8_t*>(mapped.pData);
		for (unsigned int row = 0; row < new_desc.Height; row++)
		{
			unsigned int row_start = row * mapped.RowPitch;
			for (unsigned int col = 0; col < new_desc.Width; col++)
			{
				unsigned int col_start = col * 4;

				byte R = sptr[row_start + col_start + 0];
				byte G = sptr[row_start + col_start + 1];
				byte B = sptr[row_start + col_start + 2];
				byte A = sptr[row_start + col_start + 3];

				texture->pixels_.emplace_back((int)((R + G + B + A) / 4));
			}
		}
		this->graphics_->context()->Unmap(res.Get(), 0);
	}

	texture->size_.x = static_cast<float>(desc.Width);
	texture->size_.y = static_cast<float>(desc.Height);
}

void Dx11TextureManager::CreateVertexBufferFromTextureSize(const std::shared_ptr<Texture>& texture)
{
	struct SimpleVertex
	{
		DirectX::XMFLOAT3 position_;
		DirectX::XMFLOAT2 texcoord_;
	};

	auto & x = texture->size_.x;
	auto & y = texture->size_.y;

	SimpleVertex vertices[] =
	{
		DirectX::XMFLOAT3(-x / 2.f, 0, +y / 2.f),DirectX::XMFLOAT2(0,1), //頂点3
		DirectX::XMFLOAT3(+x / 2.f, 0, +y / 2.f),DirectX::XMFLOAT2(1,1), //頂点4
		DirectX::XMFLOAT3(+x / 2.f, 0, -y / 2.f), DirectX::XMFLOAT2(1,0),//頂点2
		DirectX::XMFLOAT3(-x / 2.f, 0, -y / 2.f),DirectX::XMFLOAT2(0,0),//頂点1,
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * (sizeof(vertices) / sizeof(vertices[0]));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;

	this->graphics_->device()->CreateBuffer(&bd, &InitData, texture->vertex_buffer_.GetAddressOf());
}
