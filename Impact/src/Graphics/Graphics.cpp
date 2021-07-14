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
	}

	Graphics::~Graphics()
	{
		// consider ComPtr
		if ( m_pRenderTargetView )
		{
			m_pRenderTargetView->Release();
			m_pRenderTargetView = nullptr;
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
	}

	void Graphics::DrawTriangle()
	{
		// Vertex Setup
		struct Vertex
		{
			float x, y, z;
			float r, g, b, a;
		};

		// LineList
		//const Vertex vtx[]
		//{
		//	{0.5f, -0.5, 0.0f},		//bottom right
		//	{-0.5f, -0.5, 0.0f},	//bottom left

		//	{-0.5f, -0.5, 0.0f},	//bottom left
		//	{-0.5f, 0.5, 0.0f},		//top left

		//	{-0.5f, 0.5, 0.0f},		//top left
		//	{0.5f, -0.5, 0.0f},		//bottom right

		//	{0.5f, -0.5, 0.0f},		//bottom right
		//	{0.5f, 0.5f, 0.0f},		//top right

		//	{0.5f, 0.5f, 0.0f},		//top right
		//	{-0.5f, 0.5, 0.0f},		//top left
		//};


		// LineStrip
		//const Vertex vtx[]
		//{
		//	{  0.5f, -0.5,  0.0f,   1.0f, 0.0f, 0.0f, 1.0f},		//bottom right
		//	{ -0.5f, -0.5,  0.0f,   0.0f, 1.0f, 0.0f, 1.0f},		//bottom left
		//	{ -0.5f,  0.5,  0.0f,   0.0f, 0.0f, 1.0f, 1.0f},		//top left
		//
		//	{  0.5f, -0.5,  0.0f,   0.0f, 1.0f, 0.0f, 1.0f},		//bottom right
		//	{  0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f, 1.0f},		//top right
		//	{ -0.5f,  0.5,  0.0f,   0.0f, 0.0f, 1.0f, 1.0f},		//top left
		//};

		// TriangleList
		const Vertex vtx[]
		{
			{  0.5f, -0.5,  0.0f,   1.0f, 0.0f, 0.0f, 1.0f},		//bottom right
			{ -0.5f, -0.5,  0.0f,   0.0f, 1.0f, 0.0f, 1.0f},		//bottom left
			{ -0.5f,  0.5,  0.0f,   0.0f, 0.0f, 1.0f, 1.0f},		//top left

			{ -0.5f,  0.5,  0.0f,   0.0f, 0.0f, 1.0f, 1.0f},		//top left
			{  0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f, 1.0f},		//top right
			{  0.5f, -0.5,  0.0f,   1.0f, 0.0f, 0.0f, 1.0f},		//bottom right
		};

		D3D11_BUFFER_DESC vertexBufferDesc{};

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = sizeof(Vertex);
		vertexBufferDesc.ByteWidth = sizeof(vtx);

		D3D11_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pSysMem = vtx;

		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, m_pDevice->CreateBuffer(&vertexBufferDesc, &subResourceData, &pVertexBuffer));

		const UINT stride = sizeof(Vertex);
		const UINT offset = 0;
		m_pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

		// PixelShader
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, D3DReadFileToBlob(L"D:/Git/Impact/Impact/Resources/Shaders/PixelShader.cso", &pBlob));
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
		m_pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

		// vertex Shader
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, D3DReadFileToBlob(L"D:/Git/Impact/Impact/Resources/Shaders/VertexShader.cso", &pBlob));
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
		m_pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
	
		// Index Setup

		//const int indx[3]
		//{
		//	0, 2, 1
		//};

		//D3D11_BUFFER_DESC indexBufferDesc{};

		//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//indexBufferDesc.CPUAccessFlags = 0;
		//indexBufferDesc.MiscFlags = 0;
		//indexBufferDesc.StructureByteStride = sizeof(int);
		//indexBufferDesc.ByteWidth = sizeof(indx);

		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayour;
		const D3D11_INPUT_ELEMENT_DESC ied[]
		{
			{"SV_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__,
				m_pDevice->CreateInputLayout(
					ied,
					(UINT) std::size(ied),
					pBlob->GetBufferPointer(),
					pBlob->GetBufferSize(),
					&pInputLayour));

		m_pDeviceContext->IASetInputLayout(pInputLayour.Get());

		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);


		//m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );


		D3D11_VIEWPORT viewport{};
		viewport.Width = 1280;
		viewport.Height = 720;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
	
		m_pDeviceContext->RSSetViewports(1, &viewport);
		m_pDeviceContext->Draw((UINT) std::size(vtx), 0);
	}

	void Graphics::Present()
	{
		// Present the bacl buffer to the screen since rendering is complete.
		// Lock to screen refresh rate.
		HRESULT hr = m_pSwapChain->Present((UINT)m_VSyncEnabled, 0u);

		if ( FAILED(hr) )
			GFX_DEVICE_REMOVED_EXCEPTION(__LINE__, __FILE__, hr, m_pDevice);
	}
}