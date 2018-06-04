// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#define WIN32_LEAN_AND_MEAN // Windows ヘッダーから使用されていない部分を除外します。
#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

