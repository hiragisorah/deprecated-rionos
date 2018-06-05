#pragma once

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#include <DirectXMath.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "renderer.h"
#include "window.h"

enum FRAME_RATE
{
	FRAME_RATE_UNLIMITED,
	FRAME_RATE_LIMITED
};

struct IShader {};
struct ITexture
{
	ITexture(void)
		: size_(DirectX::XMFLOAT2(0, 0))
	{}

	DirectX::XMFLOAT2 size_;
};
struct IModel {};

class Graphics
{
public:
	Graphics(Window * const window) : window_(window) {}
	virtual ~Graphics(void) {}

protected:
	Window * const window_;

public:
	Window * const window(void) const;

public:
	virtual void Initialize(void) = 0;
	virtual void Finalize(void) = 0;

	virtual void Clear(void) = 0;
	virtual void Present(void) = 0;

	virtual void Rendering(const std::weak_ptr<Renderer> & renderer) = 0;

public:
	bool Run(void);

public:
	virtual void BackBuffer2D(void) = 0;
	virtual void BackBuffer3D(void) = 0;
	virtual void BackBufferDisplacement(void) = 0;
	virtual void Deffered2D(void) = 0;
	virtual void Deffered3D(void) = 0;
	virtual void DefferedDisplacement(void) = 0;
	virtual void ShadowMap(void) = 0;
	virtual void ShadowMapDisplacement(void) = 0;

	// DB
private:
	std::unordered_map<Resource::Shader::PATH, std::shared_ptr<IShader>> shader_db_;
	std::unordered_map<Resource::Texture::PATH, std::shared_ptr<ITexture>> texture_db_;
	std::unordered_map<Resource::Model::PATH, std::shared_ptr<IModel>> model_db_;

public:
	void LoadShader(Resource::Shader::PATH path)
	{
		auto & shader = this->shader_db_[path];
		this->LoadShader(path, shader);
	}
	void UnloadShader(Resource::Shader::PATH path)
	{
		this->shader_db_.erase(path);
	}
	const std::shared_ptr<IShader> & GetShader(Resource::Shader::PATH path)
	{
		return this->shader_db_[path];
	}
	template<class _Shader> const std::shared_ptr<_Shader> GetShader(Resource::Shader::PATH path)
	{
		return std::static_pointer_cast<_Shader>(this->shader_db_[path]);
	}

public:
	void LoadTexture(Resource::Texture::PATH path)
	{
		auto & texture = this->texture_db_[path];
		this->LoadTexture(path, texture);
	}
	void UnloadTexture(Resource::Texture::PATH path)
	{
		this->texture_db_.erase(path);
	}
	const std::shared_ptr<ITexture> & GetTexture(Resource::Texture::PATH path)
	{
		return this->texture_db_[path];
	}
	template<class _Texture> const std::shared_ptr<_Texture> GetTexture(Resource::Texture::PATH path)
	{
		return std::static_pointer_cast<_Texture>(this->texture_db_[path]);
	}

public:
	void LoadModel(Resource::Model::PATH path)
	{
		auto & model = this->model_db_[path];
		this->LoadModel(path, model);
	}
	void UnloadModel(Resource::Model::PATH path)
	{
		this->model_db_.erase(path);
	}
	const std::shared_ptr<IModel> & GetModel(Resource::Model::PATH path)
	{
		return this->model_db_[path];
	}
	template<class _Model> const std::shared_ptr<_Model> GetModel(Resource::Model::PATH path)
	{
		return std::static_pointer_cast<_Model>(this->model_db_[path]);
	}

public:
	virtual void LoadShader(const Resource::Shader::PATH & path, std::shared_ptr<IShader> & shader) = 0;
	virtual void LoadTexture(const Resource::Texture::PATH & path, std::shared_ptr<ITexture> & texture) = 0;
	virtual void LoadModel(const Resource::Model::PATH & path, std::shared_ptr<IModel> & model) = 0;

private:
	std::unordered_map<DRAW_MODE, std::vector<std::weak_ptr<Renderer>>> renderer_list_;

public:
	void AddRenderer(const std::shared_ptr<Renderer> & renderer)
	{
		this->renderer_list_[renderer->draw_mode_].emplace_back(renderer);
	}

public:
	virtual void Destroy(void) = 0;
};