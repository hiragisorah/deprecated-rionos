#pragma once

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

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
	virtual void Deffered2D(void) = 0;
	virtual void Deffered3D(void) = 0;
	virtual void ShadowMap(void) = 0;

private:
	std::unordered_map<DRAW_MODE, std::vector<std::weak_ptr<Renderer>>> renderer_list_;

public:
	void AddRenderer(const std::shared_ptr<Renderer> & renderer);

public:
	virtual void Destroy(void) = 0;
};