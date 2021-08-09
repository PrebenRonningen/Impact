#include "Graphics.h"
#include <DirectXMath.h>
#include <sal.h>
#include <d3dcompiler.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")


#include "Core/Exceptions/GraphicsThrow.h"

namespace Impact
{
	Graphics::Graphics(HWND hwnd, uint16_t width, uint16_t height)
		: m_VSyncEnabled{ true }
		, m_ViewportWidth{ width }
		, m_ViewportHeight{ height }
		, m_WindowHandle{ hwnd }
	{
		// Create a DirectX graphics interface factory.
		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory{};

		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__,
			CreateDXGIFactory(__uuidof( IDXGIFactory ), &pFactory ));

		// Use the factory to create an adapter array for the primary graphics interface (video card).
	//To get ALL possible adaptors do This \/
		{
			//UINT i = 0;
			//IDXGIAdapter* pAdapter{};
			//std::vector<IDXGIAdapter*> vAdapters;
			//// finds all adapters, if vAdapters.size() > 1, idx 0 is primary gpu, others may be integrated gpus(gpu in motherboard)
			//while ( pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND )
			//{
			//	vAdapters.push_back(pAdapter);
			//	++i;
			//}
			//i = 0;
			//IDXGIOutput* pOutput{};
			//std::vector<IDXGIOutput*> vOutputs;
			//for ( IDXGIAdapter* adapter : vAdapters )
			//{
			//	// fills the array with currently connected/available display outputs
			//	while ( adapter->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND )
			//	{
			//		// primary display = idx 0
			//		vOutputs.push_back(pOutput);
			//		++i;
			//	}
			//}
		}
		// Only interested in primary.
		Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter{};
		
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, 
			pFactory->EnumAdapters(0, &pAdapter));

		Microsoft::WRL::ComPtr<IDXGIOutput> pOutput{};
		pAdapter->EnumOutputs(0, &pOutput);

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		// idx 0 for primary display
		uint32_t numModes{};

		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, 
			pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
		if ( !displayModeList )
		{
			DebugBreak();
			return;
		}

		// Now fill the display mode list structures.
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, 
			pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList));

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator for the refresh rate for that monitor.
		uint32_t numerator{};
		uint32_t denominator{};
		DXGI_MODE_DESC modeDesc;
		for ( uint32_t i = 0; i < numModes; ++i )
		{
			modeDesc = *( displayModeList + i );
			// TODO: displayModeList[i].Width/Hwight gives off warning: C6385
			// fix ( displayModeList + i )-> to get rid of warning: C6385
			if ( ( displayModeList + i )->Width == width )
			{
				if ( ( displayModeList + i )->Height == height )
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
					break;
				}
			}
		}

		modeDesc.RefreshRate.Numerator = ( m_VSyncEnabled ) ? numerator : 0;
		modeDesc.RefreshRate.Denominator = ( m_VSyncEnabled ) ? denominator : 0;


#pragma region release the structures and interfaces used to get information about video card 
		// Release the display mode list.
		delete[] displayModeList;
		displayModeList = nullptr;

		//// Release the output
		//pOutput->Release();
		//pOutput = nullptr;
		//
		//// Release the adapter.
		//pAdapter->Release();
		//pAdapter = nullptr;
		//
		//// Release the factory.
		//pFactory->Release();
		//pFactory = nullptr;
#pragma endregion

		hwnd;
		DXGI_SAMPLE_DESC sampleDesc{};
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;


		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapChainDesc.BufferDesc = modeDesc;
		swapChainDesc.SampleDesc.Count = 1;		// No Anti-Aliasing for now
		swapChainDesc.SampleDesc.Quality = 0;	// No Anti-Aliasing for now
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //  MISCELLANEOUS WARNING #294:
		swapChainDesc.Flags = 0;



		const D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		int numFeatureLevels = ARRAYSIZE(featureLevels);

		UINT swapCreateFlags = 0;

	#ifdef _DEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif // DEBUG

	
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__,
		D3D11CreateDeviceAndSwapChain(
			 nullptr,
			 D3D_DRIVER_TYPE_HARDWARE,
			 nullptr,
			 swapCreateFlags,
			 featureLevels,
			 numFeatureLevels,
			 D3D11_SDK_VERSION,
			 &swapChainDesc,
			 &m_pSwapChain,
			 &m_pDevice,
			 nullptr,
			 &m_pDeviceContext));

		// Get the pointer to the back buffer.
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer{};
		//	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		//	hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, 
			m_pSwapChain->GetBuffer(0, __uuidof( ID3D11Resource ), &pBackBuffer));

		if ( pBackBuffer != nullptr ) // checking if NULL to get rid of Warning C6387
		{
			GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, 
				m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRenderTargetView));
		}

		//pBackBuffer->Release();
		//pBackBuffer = nullptr;

		//Depth Buffer Setup
		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{};
		
		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		
		//depthStencilDesc.StencilEnable = true;
		//depthStencilDesc.StencilReadMask = 0xFF;
		//depthStencilDesc.StencilWriteMask = 0xFF;
		//
		//// Stencil operations if pixel is front-facing.
		//depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		//depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//
		//// Stencil operations if pixel is back-facing
		//depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		//depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState{};
		
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__,
			m_pDevice->CreateDepthStencilState(&depthStencilStateDesc, &pDepthStencilState));
		m_pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1); // try changing this
		
		// Depth Stencil Buffer setup
		
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC depthStencilDesc{};
		
		depthStencilDesc.Width = m_ViewportWidth;
		depthStencilDesc.Height = m_ViewportHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;
		
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__,
			m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &pDepthStencil));
		
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		
		depthStencilViewDesc.Format = depthStencilDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__,
			m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &depthStencilViewDesc, &m_pDepthStencilView));
		
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

		// viewport
		D3D11_VIEWPORT viewport{};
		viewport.Width = m_ViewportWidth;
		viewport.Height = m_ViewportHeight;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		m_pDeviceContext->RSSetViewports(1, &viewport);
	}

	Graphics::~Graphics()
	{
		// consider ComPtr
		if ( m_pDepthStencilView )
		{
			m_pDepthStencilView->Release();
			m_pDepthStencilView = nullptr;
		}

		if ( m_pRenderTargetView )
		{
			m_pRenderTargetView->Release();
			m_pRenderTargetView = nullptr;
		}

		if (m_pRasterState)
		{
			m_pRasterState->Release();
			m_pRasterState = nullptr;
		}

		if ( m_pDeviceContext )
		{
			m_pDeviceContext->Release();
			m_pDeviceContext = nullptr;
		}

		if ( m_pSwapChain )
		{
			m_pSwapChain->Release();
			m_pSwapChain = nullptr;
		}

		if ( m_pDevice )
		{
			m_pDevice->Release();
			m_pDevice = nullptr;
		}
	}

	void Graphics::ClearBuffer(const DirectX::XMFLOAT4& color) noexcept
	{
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, &color.x);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	}

	void Graphics::DrawIndexed(const uint32_t count) const noexcept
	{
		m_pDeviceContext->DrawIndexed(count, 0, 0);
	}

	void Graphics::Present()
	{
		// Present the bacl buffer to the screen since rendering is complete.
		// Lock to screen refresh rate.
		HRESULT hr = m_pSwapChain->Present((UINT)m_VSyncEnabled, 0u);

		if ( FAILED(hr) )
			GFX_DEVICE_REMOVED_EXCEPTION(__LINE__, __FILE__, hr, m_pDevice);
	}
	DirectX::XMFLOAT4X4 Graphics::GetProjection() const noexcept
	{
		float aspectRation = ( float(m_ViewportHeight) / m_ViewportWidth );
		DirectX::XMFLOAT4X4 projMatrix;
		DirectX::XMStoreFloat4x4(&projMatrix, DirectX::XMMatrixPerspectiveLH(1.0f, aspectRation, 0.5f, 2000.0f));
		return projMatrix;
	}
	void Graphics::SetState(int state)
	{
		DirectX::XMVECTOR p0{-0.5f,0,0};
		DirectX::XMVECTOR p1{-0.25f,0.5f,0};
		DirectX::XMVECTOR p2{0.25f,0.5f,0};
		DirectX::XMVECTOR p3{0.5f,0,0};


		DirectX::XMFLOAT3 vecP;


		DirectX::XMStoreFloat3(&vecP, DirectX::XMVectorCatmullRom(p0,p1,p2,p3, 0.5f));



		

		switch (state)
		{
			case 0:
				{
					D3D11_RASTERIZER_DESC rasterizerDesc{};
					rasterizerDesc.FillMode = D3D11_FILL_SOLID;
					rasterizerDesc.CullMode = D3D11_CULL_BACK;
					rasterizerDesc.FrontCounterClockwise = false;
					rasterizerDesc.DepthClipEnable = true;

					GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterState));
					m_pDeviceContext->RSSetState(m_pRasterState);
				}
			break;
			case 1:
				{
					D3D11_RASTERIZER_DESC rasterizerDesc{};
					rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
					rasterizerDesc.CullMode = D3D11_CULL_NONE;
					rasterizerDesc.FrontCounterClockwise = false;
					rasterizerDesc.DepthClipEnable = true;

					GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterState));
					m_pDeviceContext->RSSetState(m_pRasterState);
				}
			break;
			default:
				{
					m_pDeviceContext->RSSetState(nullptr);
					
				}
			break;
		}

	}
}