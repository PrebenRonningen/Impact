#include "SamplerState.h"

// TODO: make it add Enum for TextureWrap, Pipeline stages and Filter settings.

namespace Impact
{
	SamplerState::SamplerState(Graphics& gfx)
	{
		D3D11_SAMPLER_DESC SamplerDesc{};
		SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreateSamplerState(&SamplerDesc, &m_pSamplerState));
	}

	void SamplerState::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	}
}