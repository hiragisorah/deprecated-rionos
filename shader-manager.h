#pragma once

#include <memory>
#include <unordered_map>

#include "resource.h"

struct IShader {};

class ShaderManager
{
private:
	std::unordered_map<Resource::Shader::PATH, std::shared_ptr<IShader>> shader_db_;

public:
	void Load(Resource::Shader::PATH path)
	{
		auto & shader = this->shader_db_[path];
		this->LoadShader(path, shader);
	}
	void Unload(Resource::Shader::PATH path)
	{
		this->shader_db_.erase(path);
	}
	const std::shared_ptr<IShader> & Get(Resource::Shader::PATH path)
	{
		return this->shader_db_[path];
	}
	template<class _Shader> const std::shared_ptr<_Shader> Get(Resource::Shader::PATH path)
	{
		return std::static_pointer_cast<_Shader>(this->shader_db_[path]);
	}

public:
	virtual void LoadShader(const Resource::Shader::PATH & path, std::shared_ptr<IShader> & shader) = 0;
};