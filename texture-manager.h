#pragma once

#include <memory>
#include <unordered_map>

#include <d3d11.h>
#include <wrl\client.h>
#include <DirectXMath.h>

#include "resource.h"

struct ITexture
{
	ITexture(void)
		: size_(DirectX::XMFLOAT2(0, 0))
	{}

	DirectX::XMFLOAT2 size_;
	std::vector<unsigned int> pixels_;
};

class TextureManager
{
private:
	std::unordered_map<Resource::Texture::PATH, std::shared_ptr<ITexture>> texture_db_;

public:
	void Load(Resource::Texture::PATH path)
	{
		auto & texture = this->texture_db_[path];
		this->LoadTexture(path, texture);
	}
	void Unload(Resource::Texture::PATH path)
	{
		this->texture_db_.erase(path);
	}
	const std::shared_ptr<ITexture> & Get(Resource::Texture::PATH path)
	{
		return this->texture_db_[path];
	}
	template<class _Texture> const std::shared_ptr<_Texture> Get(Resource::Texture::PATH path)
	{
		return std::static_pointer_cast<_Texture>(this->texture_db_[path]);
	}

private:
	virtual void LoadTexture(const Resource::Texture::PATH & path, std::shared_ptr<ITexture> & texture) = 0;
};