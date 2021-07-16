#pragma once
#include "Graphics/Bindable/Bindable.h"
namespace Impact
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode);
		virtual void Bind(Graphics& gfx) noexcept override;
	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_Layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	};
}

