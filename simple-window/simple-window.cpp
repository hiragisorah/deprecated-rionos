#include "simple-window.h"

void SimpleWindow::Destroy(void)
{
	delete this;
}

Window * Create(std::string caption, unsigned int width, unsigned int height)
{
	return new SimpleWindow(caption, width, height);
}

SimpleWindow::SimpleWindow(std::string caption, unsigned int width, unsigned int height)
	: caption_(caption), width_(width), height_(height) {}

void SimpleWindow::Initialize(void)
{
	{// ハンドル取得
		this->hinstance_ = GetModuleHandleA(nullptr);
	}

	{// ウィンドウクラスの登録
		WNDCLASSEX  wc;
		memset(&wc, 0, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = this->hinstance_;
		wc.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
		wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		wc.lpszClassName = this->caption_.c_str();
		wc.hIconSm = LoadIconA(nullptr, IDI_APPLICATION);
		RegisterClassExA(&wc);
	}

	{// ウィンドウの大きさの再調整、中央配置計算
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = static_cast<long>(this->width_);
		rc.bottom = static_cast<long>(this->height_);

		AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, true, 0);

		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;

		RECT rc_desk;
		GetWindowRect(GetDesktopWindow(), &rc_desk);

		int x = rc_desk.right / 2 - w / 2;
		int y = rc_desk.bottom / 2 - h / 2;

		{// ウィンドウの生成、表示
			this->hwnd_ = CreateWindowExA(0, this->caption_.c_str(), this->caption_.c_str(), WS_OVERLAPPEDWINDOW,
				x, y, w, h, 0, 0, this->hinstance_, 0);

			ShowWindow(this->hwnd_, SW_SHOW);
		}
	}
}

bool SimpleWindow::Run(void)
{
	return this->MessageLoop();
}

void SimpleWindow::Finalize(void)
{
}

HWND SimpleWindow::hwnd(void) const
{
	return this->hwnd_;
}

unsigned int SimpleWindow::width(void) const
{
	return this->width_;
}

unsigned int SimpleWindow::height(void) const
{
	return this->height_;
}

bool SimpleWindow::MessageLoop(void)
{
	MSG msg = { 0 };
	memset(&msg, 0, sizeof(msg));

	while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
		if (msg.message == WM_QUIT) return false;
	}
	return true;
}

LRESULT SimpleWindow::WndProc(HWND hwnd, UINT i_msg, WPARAM w_param, LPARAM l_param)
{
	switch (i_msg)
	{
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_ACTIVATEAPP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP: break;
	default:
		return DefWindowProc(hwnd, i_msg, w_param, l_param);
	}
	return 0;
}
