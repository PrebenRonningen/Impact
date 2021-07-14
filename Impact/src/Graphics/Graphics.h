#pragma once
#include "Core/ImpactWin.h"
// doesn't seem like dxgi.h is needed if #pragma comment(lib, "dxgi.lib") is used
//#include <dxgi.h>	
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
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
		void ClearBuffer(const DirectX::XMFLOAT4& color) noexcept;
		void DrawTriangle();

		void Present();		
		void VSynchOnOff()
		{
			m_VSyncEnabled = !m_VSyncEnabled;
		}; // temp
	private:
		bool m_VSyncEnabled;
	
		ID3D11Device* m_pDevice = nullptr;
		IDXGISwapChain* m_pSwapChain = nullptr;
		ID3D11DeviceContext* m_pDeviceContext = nullptr;
		ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	};
}