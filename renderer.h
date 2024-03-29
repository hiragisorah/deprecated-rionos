#pragma once

#include "resource.h"

enum DRAW_MODE
{
	DRAW_MODE_BACK_BUFFER_2D,
	DRAW_MODE_BACK_BUFFER_3D,
	DRAW_MODE_BACK_BUFFER_DISP,
	DRAW_MODE_DEFFERED_2D,
	DRAW_MODE_DEFFERED_3D,
	DRAW_MODE_DEFFERED_DISP,
	DRAW_MODE_SHADOW_MAP,
	DRAW_MODE_SHADOW_MAP_DISP
};

enum RASTERIZER_STATE
{
	RASTERIZER_STATE_CULL_NONE = 0,
	RASTERIZER_STATE_CW,
	RASTERIZER_STATE_CCW,
	RASTERIZER_STATE_WIREFRAME,
	RASTERIZER_STATE_NUM
};

enum BLEND_STATE
{
	BLEND_STATE_OPAQUE = 0,
	BLEND_STATE_ALPHA_BLEND,
	BLEND_STATE_ADD,
	BLEND_STATE_NON_PREMULTIPLIED,
	BLEND_STATE_NUM
};

enum SAMPLER_STATE
{
	SAMPLER_STATE_POINT_WRAP = 0,
	SAMPLER_STATE_POINT_CLAMP,
	SAMPLER_STATE_LINEAR_WRAP,
	SAMPLER_STATE_LINEAR_CLAMP,
	SAMPLER_STATE_ANISOTROPIC_WRAP,
	SAMPLER_STATE_ANISOTROPIC_CLAMP,
	SAMPLER_STATE_NUM
};

using TexturePath = Resource::Texture::PATH;
using ShaderPath = Resource::Shader::PATH;
using ModelPath = Resource::Model::PATH;

struct Renderer
{
	Renderer(void)
		: draw_mode_(DRAW_MODE_BACK_BUFFER_DISP)
		, rasterizer_state_(RASTERIZER_STATE_CCW)
		, blend_state_(BLEND_STATE_OPAQUE)
		, sampler_state_(SAMPLER_STATE_POINT_WRAP)
		, shader_(ShaderPath::tesselation)
		, texture_2d_({ TexturePath::Simple, TexturePath::Simple_normal })
		, model_(ModelPath::jeep)
		, constant_buffer_(nullptr)
	{}

	DRAW_MODE draw_mode_;
	RASTERIZER_STATE rasterizer_state_;
	BLEND_STATE blend_state_;
	SAMPLER_STATE sampler_state_;
	std::vector<Resource::Texture::PATH> texture_2d_;
	Resource::Shader::PATH shader_;
	Resource::Model::PATH model_;
	void * constant_buffer_;
};