#pragma once

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#include <DirectXMath.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "renderer.h"
#include "window.h"
#include "shader-manager.h"
#include "texture-manager.h"

enum FRAME_RATE
{
	FRAME_RATE_UNLIMITED,
	FRAME_RATE_LIMITED
};

struct IModel {};

class Graphics
{
public:
	Graphics(Window * const window)
		: window_(window)
	{}
	virtual ~Graphics(void) {}

protected:
	Window * const window_;

public:
	Window * const window(void) const { return this->window_; }

protected:
	std::unique_ptr<ShaderManager> shader_manager_;
	std::unique_ptr<TextureManager> texture_manager_;

public:
	const std::unique_ptr<ShaderManager> & shader_manager(void) const { return this->shader_manager_; }
	template<class _ShaderManager, class ... Args> void set_shader_manager(const Args &... args) { this->shader_manager_ = std::make_unique<_ShaderManager>(args ...); }

public:
	const std::unique_ptr<TextureManager> & texture_manager(void) const { return this->texture_manager_; }
	template<class _TextureManager, class ... Args> void set_texture_manager(const Args &... args) { this->texture_manager_ = std::make_unique<_TextureManager>(args ...); }

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
	std::unordered_map<Resource::Model::PATH, std::shared_ptr<IModel>> model_db_;

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