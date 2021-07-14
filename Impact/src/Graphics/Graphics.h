#pragma once
#include "Core/ImpactWin.h"
// doesn't seem like dxgi.h is needed if #pragma comment(lib, "dxgi.lib") is used
//#include <dxgi.h>	
#include <d3d11.h>
//#include <d3dcompiler.h>	// not needed yet

#include <wrl.h>

#include <vector>
namespace Impact
{
	class Graphics
	{
	public:
		Graphics(HWND hwnd, uint16_t width, uint16_t height);
	#pragma region delete
	Graphics(const Graphics& other) = delete;
	Graphics(Graphics&& other) = delete;
	Graphics& operator=(const Graphics& other) = delete;
	Graphics& operator=(Graphics&& other) = delete;
#pragma endregion
	
		~Graphics();
		void ClearBuffer(const float red, const float green, const float blue, const float alpha = 1.0f) noexcept;
		void Present();
	private:
		bool m_VSyncEnabled;
	
		ID3D11Device* m_pDevice = nullptr;
		IDXGISwapChain* m_pSwapChain = nullptr;
		ID3D11DeviceContext* m_pDeviceContext = nullptr;
		ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	};
}