#include "src/Core/ImpactWin.h"
#include <vld.h>
#include "src/Debug/WindowsMessageMap.h"
#include "src/Core/Window.h"
#include "src/Core/Exceptions/ImpException.h"
#include "Application.h"
#include <sstream>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	hInstance;
	hPrevInstance;
	pCmdLine;
	nCmdShow;

# pragma region run-time memory leak check for debug
//	//notify user if heap is corrupt
//	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
//
//	// Enable run-time memory leak check for debug builds.
//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//	typedef HRESULT(__stdcall* fPtr)( const IID&, void** );
//	HMODULE hDll = LoadLibrary("dxgidebug.dll");
//	fPtr DXGIGetDebugInterface = (fPtr) GetProcAddress(hDll, "DXGIGetDebugInterface");
//
//	IDXGIDebug* pDXGIDebug;
//	DXGIGetDebugInterface(__uuidof( IDXGIDebug ), (void**) &pDXGIDebug);
//	//_CrtSetBreakAlloc(3164);
//#endif
#pragma endregion

	try
	{
		return Application{}.Run();
	}
	catch ( const ImpException& e )
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch ( const std::exception& e )
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch ( ... )
	{
		MessageBoxA(nullptr, "No Description", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}