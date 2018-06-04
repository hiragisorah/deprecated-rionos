#pragma once

#include "..\window.h"

#include <string>

class __declspec(dllexport) SimpleWindow : public Window
{
public:
	SimpleWindow(std::string caption, unsigned int width, unsigned int height);

private:
	std::string caption_;
	unsigned int width_;
	unsigned int height_;
	HWND hwnd_;
	HINSTANCE hinstance_;

public:
	void Initialize(void) override;
	bool Run(void) override;
	void Finalize(void) override;

public:
	HWND hwnd(void) const override;
	unsigned int width(void) const override;
	unsigned int height(void) const override;

private:
	bool MessageLoop(void);

private:
	static LRESULT __stdcall WndProc(HWND hwnd, UINT i_msg, WPARAM w_param, LPARAM l_param);

public:
	void Destroy(void) override;
};

Window __declspec(dllexport) * const Create(std::string caption = "window", unsigned int width = 1280U, unsigned int height = 720U);