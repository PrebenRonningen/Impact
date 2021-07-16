#include "InputLayout.h"

Impact::InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
	:m_Layout{layout}
{
	
	GFX_EXCEPTION_NOINFO(__LINE__, __FILE__,
						 GetDevice(gfx)->CreateInputLayout(
							 &layout.front(),
							 (UINT) layout.size(),
							 pVertexShaderByteCode->GetBufferPointer(),
							 pVertexShaderByteCode->GetBufferSize(),
							 &m_pInputLayout));
}

void Impact::InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(m_pInputLayout.Get());
}
