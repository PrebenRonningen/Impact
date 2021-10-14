#include "Core/ImpactWin.h"
//#include <vld.h>
#include "Debug/WindowsMessageMap.h"
#include "Core/Window.h"
#include "Core/Exceptions/ImpException.h"
#include "Application.h"
#include <sstream>
#include <dxgidebug.h>
#include <sal.h>

	extern Impact::Application* Impact::CreateApplication();
	int WINAPI wWinMain(
		_In_ HINSTANCE hInstance, 
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ PWSTR pCmdLine, 
		_In_ int nCmdShow)
	{
		hInstance;
		hPrevInstance;
		pCmdLine;
		nCmdShow;
	
	# pragma region run-time memory leak check for debug
		//notify user if heap is corrupt
		HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	
		// Enable run-time memory leak check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
		typedef HRESULT(__stdcall* fPtr)( const IID&, void** );
		HMODULE hDll = LoadLibrary(L"dxgidebug.dll");
		if ( hDll != nullptr )
		{
			fPtr DXGIGetDebugInterface = (fPtr) GetProcAddress(hDll, "DXGIGetDebugInterface");
			IDXGIDebug* pDXGIDebug{};
			DXGIGetDebugInterface(__uuidof( IDXGIDebug ), (void**) &pDXGIDebug);
		}
		//_CrtSetBreakAlloc(90);
	#endif
	#pragma endregion
	
		try
		{
			auto app = Impact::CreateApplication();
			auto ret = app->Run();
			delete app;
			return ret;
		}
		catch ( const Impact::ImpException& e )
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