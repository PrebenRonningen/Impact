#include "Texture.h"
#include "Graphics\Surface.h"
namespace Impact
{
	Texture::Texture(Graphics& gfx, const Surface& surface)
	{
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = surface.GetWidth();
		textureDesc.Height = surface.GetHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // using ARGB as GDIplus uses ARGB
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA subResData{};
		subResData.pSysMem = surface.GetBufferPtr();
		subResData.SysMemPitch = surface.GetWidth() * sizeof(Surface::ColorARGB);
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture{};
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreateTexture2D(&textureDesc, &subResData, &pTexture));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_pTextureView));
	}
	void Texture::Bind(Graphics& gfx) noexcept
	{
	GetContext(gfx)->PSSetShaderResources(0, 1, m_pTextureView.GetAddressOf());
	}
}