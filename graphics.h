#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "renderer.h"

enum FRAME_RATE
{
	FRAME_RATE_UNLIMITED,
	FRAME_RATE_LIMITED
};

class Graphics
{
public:
	Graphics(void) {}
	virtual ~Graphics(void) {}

public:
	virtual void Initialize(void) = 0;
	virtual void Finalize(void) = 0;

	virtual void Clear(void) = 0;
	virtual void Present(void) = 0;

	virtual void SetupBackBuffer(void) = 0;
	virtual void SetupDeffered(void) = 0;
	virtual void SetupShadowMap(void) = 0;

	virtual void Setup2D(void) = 0;
	virtual void Setup3D(void) = 0;

private:
	std::unordered_map<DRAW_MODE, std::vector<std::weak_ptr<Renderer>>> renderer_list_;

public:
	void AddRenderer(const std::shared_ptr<Renderer> & renderer);
	bool Run(void);

public:
	virtual void Rendering(const std::weak_ptr<Renderer> & renderer) = 0;

public:
	virtual void Destroy(void) = 0;
};