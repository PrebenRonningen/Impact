#include "BlendState.h"

namespace Impact
{
	BlendState::BlendState(Graphics& gfx)
	{
		D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreateBlendState(&blendDesc, &m_pBlendState));
	}
	void BlendState::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->OMSetBlendState(m_pBlendState.Get(), m_BlendFactor, m_SampleMask);
	}
}