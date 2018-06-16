#pragma once

#include <wrl\client.h>
#include <d3d11.h>

#include <vector>

#include "..\texture-manager.h"

using namespace Microsoft::WRL;

class DirectX11;

struct Texture : public ITexture
{
	ComPtr<ID3D11ShaderResourceView> srv_;
	ComPtr<ID3D11Buffer> vertex_buffer_;
};

class Dx11TextureManager : public TextureManager
{
public:
	Dx11TextureManager(DirectX11 * const graphics);

private:
	void LoadTexture(const Resource::Texture::PATH & path, std::shared_ptr<ITexture> & texture) override;

private:
	DirectX11 * const graphics_;

private:
	void CreateTexture(const std::shared_ptr<Texture> & texture, const Resource::Texture::PATH & path);
	void CreateVertexBufferFromTextureSize(const std::shared_ptr<Texture> & texture);
};